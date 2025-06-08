// Código por Alejandro García Cortez 20231109
// Primera lógica de posicionamiento

//Librería Marvelmind
#include <Marvelmind.h> 

//Declaramos bjetos a utilizar
MarvelmindHedge hedge; //hedgehog a declarar
PositionValuePro obj, act; //Posición objetivo y actual
RawIMUValue actCom; //Para obtener datos de brújula
char print_buffer [128];

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
  obj.x = 2;
  obj.y = -0.75;
  obj.angle = 0;

  //Declaramos Leds
  pinMode(LED11, OUTPUT);
  pinMode(LED12, OUTPUT);
  pinMode(LED21, OUTPUT);
  pinMode(LED22, OUTPUT);
}

void loop() {
  hedge.read();
  if(millis() - t1 > period){

    //Obtenemos la posición del hedgehog y la guardamos en act
    if (hedge.getPositionFromMarvelmindHedge(true, &act)) {
      Serial.print("Posición Hedge: ");
      Serial.print(" X=");
      Serial.print(act.x, res);
      Serial.print(", Y=");
      Serial.print(act.y, res);
      Serial.print(", Z=");
      Serial.print(act.z, res);
      Serial.print(", Angle=");
      Serial.print(act.angle, res);
      Serial.println();
    }

    //Obtenemos datos de brujula
    if (hedge.getRawIMUFromMarvelmindHedge(true, &actCom)) {
      sprintf(print_buffer, "Raw IMU:cX=%05d cY=%05d cZ=%05d",
              (int)actCom.compass_x, (int)actCom.compass_y, (int)actCom.compass_z);
      Serial.println(print_buffer);
    }

    float errorx, errory; //Variables de error

    errorx = obj.x - act.x;
    errory = obj.y - act.y;
    Serial.print("Errores: ");
    Serial.print(" errorX=");
    Serial.print(errorx, res);
    Serial.print(", errorY=");
    Serial.print(errory, res);
    Serial.println();
    Serial.println();

    // Errores positivos significa que tengo que avanzar enfrente/derecha
    // Errores negativos significa que tengo que avanzar atras/izquierda
    if (errory > 0){
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
