#include <OneWire.h>
#include <DallasTemperature.h>

// Definir los pines de los sensores
#define SOIL_SENSOR_PIN 36  // Pin analógico para el YL-100
#define ONE_WIRE_BUS 19  // Pin digital para el DS18B20

// Configurar OneWire y DallasTemperature para el sensor DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  // Iniciar la comunicación serial
  Serial.begin(115200);
  
  // Iniciar el sensor DS18B20
  sensors.begin();
  
  // Mensaje de inicio
  Serial.println("Sensores YL-100 y DS18B20 iniciados");
}

void loop() {
  // Llamar a la función para leer la humedad del suelo
  leerHumedadSuelo();
  
  // Llamar a la función para leer la temperatura
  leerTemperatura();
  
  // Esperar unos segundos entre lecturas
  delay(2000);
}

// Función para leer la humedad del suelo con el YL-100
void leerHumedadSuelo() {
  // Leer el valor analógico del sensor de humedad
  int soilValue = analogRead(SOIL_SENSOR_PIN);
  
  // Convertir el valor a un porcentaje (suponiendo que 0 es muy seco y 4095 es muy húmedo)
  int humedadPorcentaje = map(soilValue, 0, 4095, 0, 100);
  
  // Imprimir los resultados en el monitor serial
  Serial.print("Humedad del suelo: ");
  Serial.print(humedadPorcentaje);
  Serial.println("%");
}
