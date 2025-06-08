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

unsigned long timeold;
float resolution = 48 * 46.85 / 4   ; // Pulsos sobre revolucion https://www.pololu.com/product/4845

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

  //Configurar Interrupción
  timeold = 0;
  // digitalPinToInterrupt toma pin y regresa true si se puede interrumpir
  attachInterrupt(digitalPinToInterrupt(EnAL),leerEncoderL,RISING);
  attachInterrupt(digitalPinToInterrupt(EnAR),leerEncoderR,RISING);

}

//-----------------loop----------------
void loop() {
  // Velocidad máxima en 255
  // Velocidad mínima en 95, recomendable 100
  MRC(255);
  MLC(255);

  //Espera un segundo para el calculo de las RPM
  if (millis() - timeold >= 1000)
  {
    //Modifica las variables de la interrupción forma atómica
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      rpmL = float(pulsosL / resolution ) / (millis() - timeold) * 1000 * 60;
      rpmR = float(pulsosR / resolution ) / (millis() - timeold) * 1000 * 60;
      timeold = millis();
      pulsosL = 0;
      pulsosR = 0;
    }
    Serial.print("RPML:\t");
    Serial.print(rpmL);
    Serial.print("\t RPMR:\t");
    Serial.println(rpmR);
  }
}

//------------------Funciones----------------
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

void encoderL(){
  int a = digitalRead(EnAL);
  int b = digitalRead(EnBL);
  Serial.print(a*5);
  Serial.print(" ");
  Serial.println(b*5);
}

void encoderR(){
  int c = digitalRead(EnAR);
  int d = digitalRead(EnBR);
  Serial.print(c*5);
  Serial.print(" ");
  Serial.println(d*5);
}

//Función para la lectura del encoder
void leerEncoderL(){
    pulsosL++; 
}

void leerEncoderR(){
    pulsosR++; 
}
