#include <BH1750.h>

// Definir el pin SCL y SDA del sensor BH1750
#define SCL 19
#define SDA 18

// Crear una instancia del sensor BH1750
BH1750 lightSensor(SDA, SCL);

void setup() {
  Serial.begin(115200);
  // Inicializar el sensor BH1750
  if (!lightSensor.begin()) {
    Serial.println("Error al iniciar el sensor BH1750");
    while (1) delay(10);
  }
}

void loop() {
  // Leer los datos de luminosidad en lux desde el sensor BH1750
  uint16_t lux = lightSensor.readLightLevel();

  Serial.print("Luminosidad (lux): ");
  Serial.println(lux);

  // Esperar antes de la siguiente lectura
  delay(1000);
}