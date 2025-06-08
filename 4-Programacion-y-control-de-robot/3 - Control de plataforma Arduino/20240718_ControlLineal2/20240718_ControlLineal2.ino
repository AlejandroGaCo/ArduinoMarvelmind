#include <TimeLib.h>
#include <Marvelmind.h>
#include <util/atomic.h>





// ----------------------------------------
// ----- Declaración de variables
// ----------------------------------------

// ------ Variables para Puente H
#define EnMR 2
#define MR_A 3
#define MR_B 4
#define ML_A 5
#define ML_B 6
#define EnML 7


// ----- Clases librería Marvelmind
MarvelmindHedge Beacon;       // Hedgehogs a declarar
PositionValuePro Angulo;                // Para obtener datos de ángulo
FusionIMUValuePro Posicion;             // Para obtener datos de posición y velocidad
unsigned long t;                        // para función millis()
const unsigned long period = 125;       // tasa de refresco en mseg
long baudrate = 500000;                 // baudrate para interfaz Marvelmind


// ----- Variables para trayectoria lineal
float k1 = 0.8, k2 = 1.0, k3 = 0.25;     // constantes de proporción
#define PI 3.14159265


// ----- Variables para control 
float xd, xd_dot, xd_ddot;              // x deseadas
float yd, yd_dot, yd_ddot;              // y deseadas
float thetad, thetad_dot;               // theta deseadas
float theta_new, theta_old, theta_dot;  // cambios de theta
float u1 = 0, u2 = 0;                   // condiciones iniciales


// ----- Varaibles para motores
float wR, wL;                           // velocidad angular 
float pwmR, pwmL;                       // valores de PWM
float radio = 0.03;                    // radio ruedas en metros


// Varaibles para el tiempo
float tiempoTranscurrido, minutos, segundos;





// ----------------------------------------
// ----- Void setup, a ejecutar una vez
// ----------------------------------------
void setup() {
  // ----- Configuramos las salidas para el puente H
  pinMode(EnML, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  pinMode(EnMR, OUTPUT);

  // ----- Baudrate del sistema Marvelmind.
  Serial.begin(500000);
  Serial3.begin(baudrate);

  Beacon.begin(&Serial3, &Serial); //Declaramos objeto BeaconIzquierda
 
  t = millis();
  theta_old = 0;
}






// ----------------------------------------
// ----- Void loop, se repite 
// ----------------------------------------
void loop() {
  Beacon.read();
  if (millis() - t > period){

    // ----- Actualizamos datos 
    Beacon.getPositionFromMarvelmindHedge(true, &Angulo);
    Beacon.getFusionIMUFromMarvelmindHedge(true, &Posicion );

    // ----- Calculamos razón de cambio de theta
    theta_new = Angulo.angle * PI / 180; // rad
    theta_dot = (theta_new - theta_old ) / ((millis() - t) * 1000); // rad/s 
    theta_old = theta_new;

    // ----- Asignamos tiempo
    minutos = minute();
    segundos = second();
    tiempoTranscurrido = segundos + 60.0*minutos;
    tiempoTranscurrido = millis() / 1000.0;

   
    // ----- Calculamos ley de control
    LeyDeControlLinea(Posicion.x,Posicion.y,theta_new, tiempoTranscurrido);
    ControlMotores();

    // ----- Imprimimos datos
    impresionSerial(); 

    // ----- Actualizamos tiempo
    t = millis();
  }
}





// ----------------------------------------
// ----- Funciones adicionales
// ----------------------------------------

//------ Funciones de asignación de velocidad
void MLC(int vel){
  if (vel>0){
    analogWrite(EnML, vel);
    digitalWrite(ML_A, HIGH);
    digitalWrite(ML_B, LOW);  
  } else {
    analogWrite(EnML, abs(vel));
    digitalWrite(ML_A, LOW);
    digitalWrite(ML_B, HIGH); 
  }
}
void MRC(int vel){
  if (vel > 0){
    analogWrite(EnMR, vel);
    digitalWrite(MR_A, HIGH);
    digitalWrite(MR_B, LOW); 
  } else {
    analogWrite(EnMR, abs(vel));
    digitalWrite(MR_A, LOW);
    digitalWrite(MR_B, HIGH);
  }
}

// ----- Funcion ley de control
void LeyDeControlLinea(float x, float y, float theta, float seg){
  u1=0;
  u2=0;
  if(seg>4){
    xd = 2.8;
    xd_dot = 0;
  
    yd = 0.3 + (seg-4)/2;
    yd_dot = (1/2);
  
    if (yd > 3.75){
      yd = 3.75;
      yd_dot=0;
    }

    thetad = PI/2;
    thetad_dot = 0;


    u1 = (xd_dot - k1*(x-xd))*cos(theta) + (yd_dot - k2*(y-yd))*sin(theta);
    u2 = thetad_dot-k3*sin(theta-thetad);
  }
}

// ----- Funcion control motores
boolean ControlMotores(){  
  wL = u1/radio - u2/radio;
  wR = u1/radio + u2/radio;
  
  pwmL = (wL-1.5401)/0.0665;
  pwmR = (wR-(0.9105)*1.15)/0.0662;

  if (pwmL > 255){
    MLC(255);
  } else if (pwmL < 115) { 
    MLC(0);
  } else {
    MLC(pwmL);
  }

  if (pwmR > 255){
    MRC(255);
  } else if (pwmR < 115) { 
    MRC(0);
  }else {
    MRC(pwmR);
  }
}

// ----- Función de impresión de datos
void imprimirDatosLectura(){
    Serial.println();
    
    Serial.print("Tiempo=\t");
    Serial.println(tiempoTranscurrido);
    
    Serial.print("x=\t");
    Serial.print(Posicion.x);
    Serial.print("\tvx=\t");
    Serial.print(Posicion.vx);
    Serial.print("\ty=\t");
    Serial.print(Posicion.y);
    Serial.print("\tvy=\t");
    Serial.print(Posicion.vy);
    Serial.print("\ttheta=\t");
    Serial.print(theta_new);
    Serial.print("\ttheta_dot=\t");
    Serial.println(theta_dot);
}

void imprimirDatosControl(){
    Serial.println();
    
    Serial.print("Tiempo\t");
    Serial.println(tiempoTranscurrido);
    
    Serial.print("xd=\t");
    Serial.print(xd);
    Serial.print("\txd_dot=\t");
    Serial.print(xd_dot);
    Serial.print("\txd_ddot=\t");
    Serial.println(xd_ddot);
    
    Serial.print("yd=\t");
    Serial.print(yd);
    Serial.print("\tyd_dot=\t");
    Serial.print(yd_dot);
    Serial.print("\tyd_ddot=\t");
    Serial.println(yd_ddot);

    Serial.print("thetad=\t");
    Serial.print(thetad);
    Serial.print("\tthetad_dot=\t");
    Serial.println(thetad_dot);

    Serial.print("u1=\t");
    Serial.print(u1);
    Serial.print("\tu2=\t");
    Serial.println(u2);
}

void imprimirDatosMotores(){
  Serial.println();
  
  Serial.print("wL=\t");
  Serial.print(wL);
  Serial.print("\twR=\t");
  Serial.println(wR );
  
  Serial.print("pwmL=\t");
  Serial.print(pwmL);
  Serial.print("\tpwmR=\t");
  Serial.println(pwmR );

}

void impresionSerial(){
    Serial.print(tiempoTranscurrido);
    Serial.print(";");
    Serial.print(Posicion.x);
    Serial.print(";");
    Serial.print(Posicion.vx);
    Serial.print(";");
    Serial.print(Posicion.y);
    Serial.print(";");
    Serial.print(Posicion.vy);
    Serial.print(";");
    Serial.print(theta_new);
    Serial.print(";");
    Serial.print(theta_dot);
    Serial.print(";");
    Serial.print(xd);
    Serial.print(";");
    Serial.print(xd_dot);
    Serial.print(";");
    Serial.print(xd_ddot);
    Serial.print(";");
    Serial.print(yd);
    Serial.print(";");
    Serial.print(yd_dot);
    Serial.print(";");
    Serial.print(yd_ddot);
    Serial.print(";");
    Serial.print(thetad);
    Serial.print(";");
    Serial.print(thetad_dot);
    Serial.print(";");
    Serial.print(wL);
    Serial.print(";");
    Serial.print(wR);
    Serial.print(";");
    Serial.print(pwmL);
    Serial.print(";");
    Serial.println(pwmR);
}
