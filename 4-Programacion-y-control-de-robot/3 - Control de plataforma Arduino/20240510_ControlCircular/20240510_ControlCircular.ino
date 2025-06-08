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
FusionIMUValuePro ActualR, ActualL; // Para obtener y comparar los datos de posición

unsigned long t1; //para función millis()
const unsigned long period = 125; //tasa de refresco en mseg
long baudrate = 500000; //baudrate para interfaz Marvelmind
const int res = 2; //resolución para imprimir valores, número de decimales
const float resolution = 48 * 46.85 / 4   ; // Pulsos sobre revolucion https://www.pololu.com/product/4845

// Variables para trayectoria circular
float f = 1/10; //Frecuencia
float r = 1; // Radio
float c1 = 2; // Coordenada X centro
float c2 = 2; // Coordenada Y centro
float k1 = 2, k2 = 2, k3 = 2; 

// Variables de control
float xd, xd_dot, xd_ddot;
float yd, yd_dot, yd_ddot;
float thetad, thetad_dot;
float u1 = 0, u2 = 0; // Condiciones iniciales

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

  // Baudrate del sistema Marvelmind
  Serial.begin(500000);
  Serial2.begin(baudrate);
  Serial3.begin(baudrate);
  
  //Inicializamos los objectos hedgehogs
  BeaconR.begin(&Serial2, &Serial); //Declaramos objeto BeaconDerecho
  BeaconL.begin(&Serial3, &Serial); //Declaramos objeto BeaconIzquierda
 
  //Configurar Interrupción
   t1 = millis();
  // digitalPinToInterrupt toma pin y regresa true si se puede interrumpir
  attachInterrupt(digitalPinToInterrupt(EnAL),leerEncoderL,RISING);
  attachInterrupt(digitalPinToInterrupt(EnAR),leerEncoderR,RISING);

}

//-----------------loop----------------
void loop() {
  BeaconR.read();
  BeaconL.read();

  //Espera un segundo para el calculo de las RPM
  if (millis() - t1 > period){

    
    //Para el cálculo de las RPM
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      rpmL = float(pulsosL / resolution ) / (millis() - t1) * 1000 * 60;
      rpmR = float(pulsosR / resolution ) / (millis() - t1) * 1000 * 60;
      t1 = millis();
      pulsosL = 0;
      pulsosR = 0;
    }

    //Obtenemos la información de posición 
    BeaconR.getFusionIMUFromMarvelmindHedge(true, &ActualR);
    BeaconL.getFusionIMUFromMarvelmindHedge(true, &ActualL);

    // Calculamos razón de cambio de theta

    // Calculamos ley de control
    LeyDeControl(ActualL.x,ActualL.vx,ActualL.y,ActualL.vx
    
    // Actualizamos el valor de milis
    t1 = millis();
  }
}

//------------------Funciones----------------}

// Funciones de asignación de velocidad
void MLC(int vel){
  analogWrite(EnML, vel);
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);  
}
void MRC(int vel){
  analogWrite(EnMR, vel);
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);  
}

//Función para la calcular del encoder
void leerEncoderL(){
    pulsosL++; 
}
void leerEncoderR(){
    pulsosR++; 
}

// Funciónes de ley de control
boolean LeyDeControl(float x, float x_dot, float y, float y_dot, float theta, float theta_dot, float t){
  xd = r*sin(2*PI*f*t) + c1;
  xd_dot = (2*PI*f)*r*cos(2*PI*f*t);
  xd_ddot = -pow(2*PI*f,2)*r*sin(2*PI*f*t);

  yd = r*cos(2*PI*f*t) + c1;
  yd_dot = -(2*PI*f)*r*sin(2*PI*f*t);
  yd_ddot = -pow(2*PI*f,2)*r*cos(2*PI*f*t);

  thetad = atan2(yd_dot, xd_dot);
  thetad_dot = ((yd_ddot*xd_dot-xd_ddot*yd_dot)/pow(xd_dot,2))/(1+pow(yd_dot/xd_dot,2));

  u1 = (xd_dot - k1*(x-xd))*cos(theta) + (yd_dot - k2*(y-yd))*sin(theta);
  u2 = thetad_dot-k3*sin(theta-thetad);

  return true;
}
