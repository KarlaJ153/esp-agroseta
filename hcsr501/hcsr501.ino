
#include <HC_SR501.h>

// Definir los pines del sensor HC-SR501
#define TRIG_PIN 2
#define ECHO_PIN 3

// Crear una instancia del sensor HC-SR501
HC_SR501 hc_sr501(TRIG_PIN, ECHO_PIN);

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Leer el estado de movimiento desde el sensor HC-SR501
  int state = hc_sr501.getState();

  if (state == 0) {
    Serial.println("Sin movimiento");
  } else {
    Serial.println("Movimiento detectado");
  }

  // Esperar antes de la siguiente lectura
  delay(100);
}