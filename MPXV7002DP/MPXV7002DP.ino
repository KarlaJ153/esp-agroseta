#include <MPXV7002DP.h>

// Definir los pines del sensor MPXV7002DP
#define SCL 19
#define SDA 18

// Crear una instancia del sensor MPXV7002DP
MPXV7002DP pressureSensor(SDA, SCL);

void setup() {
  Serial.begin(115200);
  // Inicializar el sensor MPXV7002DP
  if (!pressureSensor.begin()) {
    Serial.println("Error al iniciar el sensor MPXV7002DP");
    while (1) delay(10);
  }
}

void loop() {
  // Leer la presión en hPa desde el sensor MPXV7002DP
  float pressure = pressureSensor.readPressure();

  Serial.print("Presión: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Esperar antes de la siguiente lectura
  delay(1000);
}