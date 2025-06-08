#define EnML 2
#define ML_A 3
#define ML_B 4
#define MR_A 5
#define MR_B 6
#define EnMR 7

void setup() {
  // Configuramos las salidas para el puente H
  pinMode(EnML, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  pinMode(EnMR, OUTPUT);
  
  Serial.begin(500000);
 
}

//-----------------loop----------------
void loop() {

  // Velocidad máxima en 255
  // Velocidad mínima en 95, recomendable 100
  MRC(100);
  MLC(100);
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
