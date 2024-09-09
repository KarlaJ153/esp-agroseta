
#include <OneWire.h>
#include <DallasTemperature.h>

// Definir el pin del sensor DS18B20
#define ONE_WIRE_BUS 2

// Crear una instancia de OneWire y DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

void setup() {
  Serial.begin(115200);
  // Inicializar el sensor DS18B20
  ds18b20.begin();
}

void loop() {
  // Leer la temperatura en grados Celsius desde el sensor DS18B20
  ds18b20.requestTemperatures();
  float temperature = ds18b20.getTempCByIndex(0);

  if (temperature == DEVICE_DISCONNECTED) {
    Serial.println("Error al leer la temperatura");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println("°C");

    // Esperar antes de la siguiente lectura
    delay(1000);
  }
}