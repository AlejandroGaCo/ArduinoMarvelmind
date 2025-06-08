#define EnML 15
#define ML_A 2
#define ML_B 4

#define EnMR 5
#define MR_A 16
#define MR_B 17

#define X1 35
#define X2 34

#define echoPin_F 27 
#define trigPin_F 14 

// Variables
long duration_F;
int dist_F;

char dato;
bool activacion = false;

// Configurando los parámetros de PWM
const int frecuencia = 500;
const int canal_L = 0;
const int canal_R = 1;
const int resolucion = 12;

int vel = 1638; //3500=85% duty cycle, 2048=50%
void setup() {
  pinMode(EnML, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);

  pinMode(EnMR, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);

  pinMode(X1, INPUT);
  pinMode(X2, INPUT);
  
  ledcSetup(canal_L, frecuencia, resolucion);
  ledcSetup(canal_R, frecuencia, resolucion);
  
  ledcAttachPin(EnML, canal_L);
  ledcAttachPin(EnMR, canal_R);

  Serial.begin(115200);
 
  pinMode(trigPin_F, OUTPUT);   // Se define el pin trigger como SALIDA
  pinMode(echoPin_F, INPUT);    // Se define el pin echo como entrada
}

//-----------------loop----------------
void loop() {
  if (digitalRead(X1) == HIGH && digitalRead(X2) == LOW){
    Right();
  } else if (digitalRead(X1) == LOW && digitalRead(X2) == HIGH){
    Left();
  } else if (digitalRead(X1) == LOW && digitalRead(X2) == LOW){
    Stop();
  } 
  
}

//------------------Funciones----------------

void Forward(){
  ledcWrite(canal_L, vel);
  ledcWrite(canal_R, vel);
  
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);  
  Serial.println("Forward");
}

void Back(){
  ledcWrite(canal_L, vel);
  ledcWrite(canal_R, vel);
  
  digitalWrite(ML_A, HIGH);
  digitalWrite(ML_B, LOW); 
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW); 
  Serial.println("Back");
}

void Stop(){
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, LOW);
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, LOW);
  Serial.println("Stopped");  
}

void Right(){
  ledcWrite(canal_L, vel);
  ledcWrite(canal_R, vel);
  
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW);  
  Serial.println("Right");
}

void Left(){
  ledcWrite(canal_L, vel);
  ledcWrite(canal_R, vel);
  
  digitalWrite(ML_A, HIGH);
  digitalWrite(ML_B, LOW);
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);  
  Serial.println("Left");
}

void Slow_Forward(){
  ledcWrite(canal_L, 0.45*vel);
  ledcWrite(canal_R, 0.45*vel);
    
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);  
  Serial.println("Slow_Forward");
}

void Slow_Back(){
  ledcWrite(canal_L, 0.45*vel);
  ledcWrite(canal_R, 0.45*vel);
  
  digitalWrite(ML_A, HIGH);
  digitalWrite(ML_B, LOW); 
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW); 
  Serial.println("Slow_Back");
}

void Slow_Right(){
  ledcWrite(canal_L, 0.34*vel);
  ledcWrite(canal_R, 0.34*vel);
  
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW);  
  Serial.println("Slow_Right");
}

void Slow_Left(){
  ledcWrite(canal_L, 0.34*vel);
  ledcWrite(canal_R, 0.34*vel);
  
  digitalWrite(ML_A, HIGH);
  digitalWrite(ML_B, LOW);
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);  
  Serial.println("Slow_Left");
}

int US_front(){
  digitalWrite(trigPin_F, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin_F, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_F, LOW);
  
  duration_F = pulseIn(echoPin_F, HIGH);
  dist_F = duration_F * 0.0343 / 2;

   
  return dist_F;
}
