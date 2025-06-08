// Código por Alejandro García Cortez 20231213
// Lectura de posisción, ángulo y velocidades

//Librería Marvelmind
#include <Marvelmind.h> 

//Declaramos bjetos a utilizar
MarvelmindHedge hedge; //hedgehog a declarar
FusionIMUValuePro Objective, Actual; //Para obtener los datos del cuaternion procesado
float roll, pitch, yaw; // Variables auxiliares para calcular ángulos

unsigned long t1; //para función millis()
const unsigned long period = 2000; //tasa de refresco en mseg
long baudrate = 500000; //baudrate para interfaz Marvelmind
const int res = 2; //resolución para imprimir valores, número de decimales

#define LED11 7
#define LED12 6
#define LED21 5
#define LED22 4

void setup() {
  Serial.begin(baudrate);
  Serial2.begin(baudrate);
  hedge.begin(&Serial2, &Serial); //Declaramos objeto hedge
  t1 = millis();

  //Declaramos posición obj
  Objective.x = 2;
  Objective.y = -0.75;
  Objective.yaw = 0;

  //Declaramos Leds
  pinMode(LED11, OUTPUT);
  pinMode(LED12, OUTPUT);
  pinMode(LED21, OUTPUT);
  pinMode(LED22, OUTPUT);
}

void loop() {
  hedge.read();
  if(millis() - t1 > period){

    //Obtenemos la información de IMU procesada
  if (hedge.getFusionIMUFromMarvelmindHedge(true, &Actual)) {
      Serial.println("IMU fusion: ");

      Serial.print(" X=");
      Serial.print(Actual.x, res);
      Serial.print(", Y=");
      Serial.print(Actual.y, res);
      Serial.print(", Z=");
      Serial.println(Actual.z, res);

      roll = Actual.roll * r2d;
      pitch = Actual.pitch * r2d;
      yaw = Actual.yaw * r2d;
      Serial.print("R=");
      Serial.print(roll, res);
      Serial.print(", P=");
      Serial.print(pitch, res);
      Serial.print(", YAW=");
      Serial.print(yaw, res);
      Serial.println();
    }

    float errorx, errory,erroryaw; //Variables de error

    errorx = Objective.x - Actual.x;
    errory = Objective.y - Actual.y;
    erroryaw = Objective.yaw-Actual.y;
    Serial.print("Errores: ");
    Serial.print(" errorX=");
    Serial.print(errorx, res);
    Serial.print(", errorY=");
    Serial.print(errory, res);
    Serial.println();
    Serial.println();

    // Errores positivos significa que tengo que avanzar enfrente/derecha
    // Errores negativos significa que tengo que avanzar atras/izquierda
    if (yaw < 30 && yaw > 0){
      digitalWrite(LED11, HIGH);
      digitalWrite(LED12, LOW);
      digitalWrite(LED21, HIGH);
      digitalWrite(LED22, LOW);
    } else {
      digitalWrite(LED11, LOW);
      digitalWrite(LED12, HIGH);
      digitalWrite(LED21, LOW);
      digitalWrite(LED22, HIGH);
    }
    t1 = millis();
  }
}
