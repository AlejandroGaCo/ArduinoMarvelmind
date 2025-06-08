// Código por Alejandro García Cortez 20231214
// Ahora sí, vamos a moverlo

//Librería Marvelmind
#include <Marvelmind.h> 

//Declaramos bjetos a utilizar
MarvelmindHedge BeaconR, BeaconL; //RobotMovilhog a declarar
PositionValuePro Destino, ActualR, ActualL; //Para obtener los datos del cuaternion procesado

unsigned long t1; //para función millis()
const unsigned long period = 125; //tasa de refresco en mseg
long baudrate = 500000; //baudrate para interfaz Marvelmind
const int res = 2; //resolución para imprimir valores, número de decimales

#define X1 7
#define X2 6
#define Y1 5
#define Y2 4

void setup() {
  Serial.begin(baudrate);
  Serial2.begin(baudrate);
  Serial3.begin(baudrate);
  BeaconR.begin(&Serial2, &Serial); //Declaramos objeto BeaconDerecho
  BeaconL.begin(&Serial3, &Serial); //Declaramos objeti BeaconIzquierda
  t1 = millis();

  //Declaramos Destino, de tipo posición, con la posición deseada
  Destino.x = 2.29;
  Destino.y = 0.57;
  Destino.angle = 45.0;

  //Declaramos Leds
  pinMode(X1, OUTPUT);
  pinMode(X2, OUTPUT);
  pinMode(Y1, OUTPUT);
  pinMode(Y2, OUTPUT);
}

void loop() {
  BeaconR.read();
  BeaconL.read();
  if(millis() - t1 > period){
    
    //Obtenemos la información de IMU procesada
    if (BeaconR.getPositionFromMarvelmindHedge(true, &ActualR)) {
      //Serial.println("Posición derecha actaulizada ");
    }
    //Obtenemos la información de IMU procesada
    if (BeaconL.getPositionFromMarvelmindHedge(true, &ActualL)) {
      //Serial.println("Posición izquierda actaulizada ");
    }

    float errorPosX, errorPosY, errorAngZ; //Variables de error

    errorAngZ = Destino.angle - ActualL.angle;

//    Serial.print("PosR: ");
//    Serial.print(" x1=");
//    Serial.print(ActualR.x , res);
//    Serial.print(" y1=");
//    Serial.print(ActualR.y, res);
//    Serial.print(" Ang=");
//    Serial.println(ActualR.angle, res);
//
//    Serial.print("PosL: ");
//    Serial.print(" x2=");
//    Serial.print(ActualL.x , res);
//    Serial.print(" y2=");
//    Serial.print(ActualL.y, res);
//    Serial.print(" Ang=");
//    Serial.println(ActualL.angle, res);
 
    Serial.print("Errores: ");
    Serial.print(" errorAngZ=");
    Serial.print(errorAngZ, res);
    Serial.print(" AngZ=");
    Serial.println(ActualL.angle, res);

    // Errores positivos significa que Actual < Destino entonces giro a la izquierda
    // Errores negativos significa que Actual > Destino entonces giro a la derecha.
    if (errorAngZ < -10){
      digitalWrite(X1, HIGH);
      digitalWrite(X2, LOW);
       Serial.println("Giroderecha");
    } else if (errorAngZ > 10){
      digitalWrite(X1, LOW);
      digitalWrite(X2, HIGH);
      Serial.println("Giroizquierda");
    } else {
      digitalWrite(X1, LOW);
      digitalWrite(X2, LOW);
      Serial.println("Estoy en posición");
    }
    
    t1 = millis();
  }
}

float angleCalc (float y1, float y2, float x1, float x2){
  float angle; 
  if (x1 == x2) {
    angle = 0;
    if (y1 < y2){
      angle = 180;
    } else {
      angle = 0;
    }
  } else {
    angle = atan((y1-y2)/(x1-x2))* 180 / PI;
  }
  return angle+90;
}
