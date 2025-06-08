#define EnML 2
#define ML_A 3
#define ML_B 4
#define MR_A 5
#define MR_B 6
#define EnMR 7

#define EnAL 22
#define EnBL 23
#define EnAR 24
#define EnBR 25

void setup() {
  // Configuramos las salidas para el puente H
  pinMode(EnML, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  pinMode(EnMR, OUTPUT);

  pinMode(EnAL, INPUT);
  pinMode(EnBL, INPUT);
  pinMode(EnAR, INPUT);
  pinMode(EnBR, INPUT);
  
  Serial.begin(9600);
 
}

//-----------------loop----------------
void loop() {

  // Velocidad máxima en 255
  // Velocidad mínima en 95, recomendable 100
  MRC(100);
  MLC(100);

  // Verificamos funcionalidad encoders
  encoderL();
  encoderR();
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
