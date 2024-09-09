#include <MQ135.h>

// Definir el pin del sensor MQ-135
#define MQ_PIN A0

// Crear una instancia del sensor MQ-135
MQ135 mq(MQ_PIN);

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Leer los datos del sensor MQ-135
  float ppm = mq.readPPM();

  Serial.print("Concentración de gas (ppm): ");
  Serial.println(ppm);

  // Esperar antes de la siguiente lectura
  delay(1000);
}