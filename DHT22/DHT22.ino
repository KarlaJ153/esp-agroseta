#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_DHT.h>

#define DHT_PIN 26 // Pin donde se conecta el sensor DHT22 (GPIO26 en ESP32)

DHT dht(DHT_PIN);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Inicializar el sensor DHT22
  dht.begin();
}

void loop() {
  // Leer temperatura y humedad del sensor DHT22
  sensors_event_t event;
  dht.readEvent(&event);

  float temperature = event.temperature;
  float humidity = event.humidity;

  Serial.print("Temperatura: ");
  Serial.print(temperature, 2); // Imprimir dos decimales después de la coma
  Serial.println(" °C");

  Serial.print("Humedad: ");
  Serial.print(humidity, 2); // Imprimir dos decimales después de la coma
  Serial.println("%");

  delay(2000); // Esperar 2 segundos antes de la siguiente lectura
