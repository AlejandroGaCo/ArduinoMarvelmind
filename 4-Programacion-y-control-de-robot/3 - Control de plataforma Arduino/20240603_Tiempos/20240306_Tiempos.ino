#include <TimeLib.h>

float tiempoTranscurrido, minutos, segundos;

void setup() {
  Serial.begin(9600);

}

void loop() {
  delay(100);
  minutos = minute();
  segundos = second();
  tiempoTranscurrido = segundos + 60*minutos;
  Serial.println(tiempoTranscurrido);

}
