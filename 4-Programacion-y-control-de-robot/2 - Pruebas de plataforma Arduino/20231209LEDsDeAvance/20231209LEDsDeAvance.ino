// Código por Alejandro García Cortez 20231213
// Primera lógica de movimiento

//Librería Marvelmind
#include <Marvelmind.h> 

//Declaramos bjetos a utilizar
MarvelmindHedge RobotMovil; //RobotMovilhog a declarar
FusionIMUValuePro Destino, Actual; //Para obtener los datos del cuaternion procesado

unsigned long t1; //para función millis()
const unsigned long period = 100; //tasa de refresco en mseg
long baudrate = 500000; //baudrate para interfaz Marvelmind
const int res = 2; //resolución para imprimir valores, número de decimales

#define LEDX1 7
#define LEDX2 6
#define LEDY1 5
#define LEDY2 4

void setup() {
  Serial.begin(baudrate);
  Serial2.begin(baudrate);
  RobotMovil.begin(&Serial2, &Serial); //Declaramos objeto RobotMovil
  t1 = millis();

  //Declaramos posición obj
  Destino.x = 2.29;
  Destino.y = 0.93;
  Destino.yaw = 0.8726;

  //Declaramos Leds
  pinMode(LEDX1, OUTPUT);
  pinMode(LEDX2, OUTPUT);
  pinMode(LEDY1, OUTPUT);
  pinMode(LEDY2, OUTPUT);
}

void loop() {
  RobotMovil.read();
  if(millis() - t1 > period){
    
    //Obtenemos la información de IMU procesada
    if (RobotMovil.getFusionIMUFromMarvelmindHedge(true, &Actual)) {
      Serial.println("IMU fusion actualizado ");
    }

    float errorPosX, errorPosY, errorAngZ; //Variables de error

    errorPosX = Destino.x - Actual.x;
    errorPosY = Destino.y - Actual.y;
    errorAngZ = Destino.yaw - Actual.yaw;
    
    Serial.print("Errores: ");
    Serial.print(" errorPosX=");
    Serial.print(errorPosX, res);
    Serial.print(", errorPosY=");
    Serial.print(errorPosY, res);
    Serial.print(", errorAngZ=");
    Serial.println(errorAngZ, res);

    // Errores positivos significa que tengo que avanzar enfrente/derecha
    // Errores negativos significa que tengo que avanzar atras/izquierda
    if (errorPosX > 0){
      digitalWrite(LEDX1, HIGH);
      digitalWrite(LEDX2, LOW);
    } else {
      digitalWrite(LEDX1, LOW);
      digitalWrite(LEDX2, HIGH);
    }

    if (errorPosY > 0){
      digitalWrite(LEDY1, HIGH);
      digitalWrite(LEDY2, LOW);
    } else {
      digitalWrite(LEDY1, LOW);
      digitalWrite(LEDY2, HIGH);
    }
    
    t1 = millis();
  }
}
