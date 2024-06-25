#include <TimeLib.h>
#include <Marvelmind.h>
#include <util/atomic.h>

// Variables para PuenteH
#define EnML 2
#define ML_A 3
#define ML_B 4

#define MR_A 5
#define MR_B 6
#define EnMR 7

// Variables para entradas de encoder
#define EnAL 20 // Interrupción 5
#define EnBL 23
#define EnAR 21 // Interrupción 6
#define EnBR 25

// Variable para cálculo de velocidades
volatile int pulsosL = 0; // Para velocidad
volatile int pulsosR = 0;
float rpmL, rpmR, radsL, radsR;

// Clases librería Marvelmind
MarvelmindHedge BeaconR, BeaconL; // Hedgehogs a declarar
PositionValuePro Angulo; // Para obtener datos de ángulo
FusionIMUValuePro Posicion; // Para obtener datos de posición y velocidad

unsigned long t; //para función millis()
const unsigned long period = 125; //tasa de refresco en mseg
long baudrate = 500000; //baudrate para interfaz Marvelmind
const int res = 2; //resolución para imprimir valores, número de decimales
const float resolution = 48 * 46.85 / 4   ; // Pulsos sobre revolucion https://www.pololu.com/product/4845

// Variables para trayectoria circular
float f = 0.025; //Frecuencia
float r = 1.0; // Radio
float c1 = 2.3; // Coordenada X centro
float c2 = 1.3; // Coordenada Y centro
float k1 = 2.0, k2 = 2.0, k3 = 2.0; 
#define PI 3.14159265

// Variables de control deseadas 
float xd, xd_dot, xd_ddot;
float yd, yd_dot, yd_ddot;
float thetad, thetad_dot;
// Varaibles para cálculo de theta
float theta_dot, theta, theta_old;
float u1 = 0, u2 = 0; // Condiciones iniciales
//Variables de control de motor
float wR, wL, pwmR, pwmL, rr = 0.035; // En rad/s

// Varaibles para el tiempo
float tiempoTranscurrido, minutos, segundos;

void setup() {
  // Configuramos las salidas para el puente H
  pinMode(EnML, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  pinMode(EnMR, OUTPUT);

  // Configuramos las entradas para los encoders
  pinMode(EnAL, INPUT);
  pinMode(EnBL, INPUT);
  pinMode(EnAR, INPUT);
  pinMode(EnBR, INPUT);

  // Baudrate del sistema Marvelmind.
  Serial.begin(500000);
  Serial3.begin(baudrate);
  
  //Inicializamos el beacon izquierdo, que nos da toda la información necesaria
  BeaconL.begin(&Serial3, &Serial); //Declaramos objeto BeaconIzquierda
 
  //Configurar Interrupción
   t = millis();
  // digitalPinToInterrupt toma pin y regresa true si se puede interrumpir
  attachInterrupt(digitalPinToInterrupt(EnAL),leerEncoderL,RISING);
  attachInterrupt(digitalPinToInterrupt(EnAR),leerEncoderR,RISING);

  theta_old = 0;
}

//-----------------loop----------------
void loop() {
  BeaconL.read();

  //Espera un segundo para el calculo de las RPM
  if (millis() - t > period){

    //Para el cálculo de las RPM
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      rpmL = float(pulsosL / resolution ) / (millis() - t) * 1000 * 60;
      rpmR = float(pulsosR / resolution ) / (millis() - t) * 1000 * 60;
      pulsosL = 0;
      pulsosR = 0;
    }

    //Obtenemos la información de posición, solo cuando hay cambios 
    BeaconL.getPositionFromMarvelmindHedge(true, &Angulo);
    BeaconL.getFusionIMUFromMarvelmindHedge(true, &Posicion );

    // Calculamos razón de cambio de theta
    theta = Angulo.angle * PI / 180; // rad
    theta_dot = (theta - theta_old ) / (millis() - t) *1000; // rad/s 
    theta_old = theta;

    
    minutos = minute();
    segundos = second();
    tiempoTranscurrido = segundos + 60.0*minutos;

   
    // Calculamos ley de control
    LeyDeControlLinea(Posicion.x,Posicion.y,theta, tiempoTranscurrido);
    ControlMotores1();

    imprimirDatosLectura();
    imprimirDatosControl();
    imprimirDatosMotores();

    t = millis();
  }
}

//------------------Funciones----------------}

// Funciones de asignación de velocidad
void MLC(int vel){
  if (vel>0){
    analogWrite(EnML, vel);
    digitalWrite(ML_A, LOW);
    digitalWrite(ML_B, HIGH);  
  } else {
    analogWrite(EnML, abs(vel));
    digitalWrite(ML_A, HIGH);
    digitalWrite(ML_B, LOW); 
  }
  
}
void MRC(int vel){
  if (vel > 0){
    analogWrite(EnMR, vel);
    digitalWrite(MR_A, LOW);
    digitalWrite(MR_B, HIGH); 
  } else {
    analogWrite(EnMR, abs(vel));
    digitalWrite(MR_A, HIGH);
    digitalWrite(MR_B, LOW);
  }
    
}

//Función para la calcular del encoder
void leerEncoderL(){
    pulsosL++; 
}
void leerEncoderR(){
    pulsosR++; 
}

// Funciónes de ley de control
void LeyDeControl(float x, float y, float theta, float seg){ 
  xd = r*sin(2*PI*f*seg) + c1;
  xd_dot = (2*PI*f)*r*cos(2*PI*f*seg);
  xd_ddot = -pow(2*PI*f,2)*r*sin(2*PI*f*seg);

  yd = r*cos(2*PI*f*seg) + c2;
  yd_dot = -(2*PI*f)*r*sin(2*PI*f*seg);
  yd_ddot = -pow(2*PI*f,2)*r*cos(2*PI*f*seg);

  thetad = atan2(yd_dot, xd_dot);
  thetad_dot = (yd_ddot*xd_dot-xd_ddot*yd_dot)/pow(xd_dot,2)/(1+pow(yd_dot/xd_dot,2));

  u1 = (xd_dot - k1*(x-xd))*cos(theta) + (yd_dot - k2*(y-yd))*sin(theta);
  u2 = thetad_dot-k3*sin(theta-thetad);

}

void LeyDeControlLinea(float x, float y, float theta, float seg){
  xd = 3.3;
  xd_dot = 0;
  
  yd = 0.3 + seg;
  yd_dot = 1;

  thetad = PI /2;

  u1 = (xd_dot - k1*(x-xd))*cos(theta) + (yd_dot - k2*(y-yd))*sin(theta);
  u2 = thetad_dot-k3*sin(theta-thetad);
}

// Control de motores
boolean ControlMotores2(){  
  wL = u1/rr - u2/rr;
  wR = u1/rr + u2/rr;
  
  pwmL = (wL - 14.264)/0.0211;
  pwmR = (wR - 14.109)/0.0215;

  if (pwmL > 255){
    MLC(255);
  } else if (abs(pwmL) < 100) { 
    MLC(0);
  } else if (pwmL < -255){
    MLC(-255);
  } else {
    MLC(pwmL);
  }

  if (pwmR > 255){
    MRC(255);
  } else if (abs(pwmR) < 100) { 
    MRC(0);
  }else if (pwmR < -255){
    MRC(-255);
  }else {
    MRC(pwmR);
  }
}

boolean ControlMotores1(){  
  wL = u1/rr - u2/rr;
  wR = u1/rr + u2/rr;
  
  pwmL = (wL - 14.264)/0.0211;
  pwmR = (wR - 14.109)/0.0215;

  if (pwmL > 255){
    MLC(255);
  } else if (pwmL < 100) { 
    MLC(0);
  } else {
    MLC(pwmL);
  }

  if (pwmR > 255){
    MRC(255);
  } else if (pwmR < 100) { 
    MRC(0);
  }else {
    MRC(pwmR);
  }
}

// Función de verificación de datos
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
    Serial.print(theta);
    Serial.print("\ttheta_dot=\t");
    Serial.println(theta_dot);
}

// Función de verificación de datos
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
