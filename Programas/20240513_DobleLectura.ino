#include <Marvelmind.h> // funciona
MarvelmindHedge BeaconL, BeaconR;

unsigned long t1;
const unsigned long period = 125;
long baudrate = 500000;

FusionIMUValuePro Actual;
PositionValuePro Angulo; // Para obtener datos de ángulo

void setup() {
  Serial.begin(baudrate);
  Serial2.begin(baudrate);
  Serial3.begin(baudrate);
  
  BeaconR.begin(&Serial2, &Serial); //Declaramos objeto BeaconDerecho
  BeaconL.begin(&Serial3, &Serial); //Declaramos objeti BeaconIzquierda

  t1 = millis();
}

void loop() {
  BeaconL.read();
  BeaconR.read();
  
  if(millis() - t1 > period){
    BeaconL.getPositionFromMarvelmindHedge(true, &Angulo);
    BeaconL.getFusionIMUFromMarvelmindHedge(false, &Actual);

    Serial.print("Angulo: ");
    Serial.println(Angulo.angle);
    t1 = millis();
  }
}
