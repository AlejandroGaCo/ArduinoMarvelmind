// Código por Alejandro García Cortez 20231214
// Ahora sí, vamos a moverlo

//Librería Marvelmind
#include <Marvelmind.h> 

//Declaramos bjetos a utilizar
MarvelmindHedge RobotMovil; //RobotMovilhog a declarar
PositionValuePro Destino, Actual; //Para obtener los datos del cuaternion procesado

unsigned long t1; //para función millis()
const unsigned long period = 1/8*1000; //tasa de refresco en mseg
long baudrate = 500000; //baudrate para interfaz Marvelmind
const int res = 2; //resolución para imprimir valores, número de decimales

#define X1 7
#define X2 6
#define Y1 5
#define Y2 4

void setup() {
  Serial.begin(baudrate);
  Serial2.begin(baudrate);
  RobotMovil.begin(&Serial2, &Serial); //Declaramos objeto RobotMovil
  t1 = millis();

  //Declaramos Destino, de tipo posición, con la posición deseada
  Destino.x = 2.29;
  Destino.y = 0.57;
  Destino.angle = 180.0;

  //Declaramos Leds
  pinMode(X1, OUTPUT);
  pinMode(X2, OUTPUT);
  pinMode(Y1, OUTPUT);
  pinMode(Y2, OUTPUT);
}

void loop() {
  RobotMovil.read();
  if(millis() - t1 > period){
    
    //Obtenemos la información de IMU procesada
    if (RobotMovil.getPositionFromMarvelmindHedge(true, &Actual)) {
      Serial.println("Posición actualizado ");
    }

    float errorPosX, errorPosY, errorAngZ; //Variables de error

    errorPosX = Destino.x - Actual.x;
    errorPosY = Destino.y - Actual.y;
    errorAngZ = Destino.angle - Actual.angle;
    
    Serial.print("Errores: ");
    Serial.print(" errorAngZ=");
    Serial.print(errorAngZ, res);
    Serial.print(" AngZ=");
    Serial.println(Actual.angle, res);

    // Errores positivos significa que tengo que avanzar enfrente/derecha. Error está en metros.
    // Errores negativos significa que tengo que avanzar atras/izquierda. Error está en metros.
    if (abs(errorAngZ) > 15){
      digitalWrite(X1, HIGH);
      digitalWrite(X2, HIGH);
      Serial.print("Giroderecha");
    } else {
      digitalWrite(X1, LOW);
      digitalWrite(X2, LOW);
      Serial.print("Estoy en posición");
    }
    
    t1 = millis();
  }
}
