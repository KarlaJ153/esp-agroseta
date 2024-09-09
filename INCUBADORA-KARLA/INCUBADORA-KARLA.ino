#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//-------------------
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//---------------SENSORES------------ntp 

const char* ssid = "DANTEJONAS";
const char* password = "Jonas2024";

DHT dht(4, DHT22);
float humidity_C = 0;
float temperature_C = 0;
char dateTime[20];

BH1750 lightMeter;
float lux = 0;
float voltageMQ = 0;
float temperaturaC = 0;
int humedadPorcentaje = 0;
float weight = 0;
bool mov = false;

#define MPXV7002DP_PIN 34
float pressure = 0;

#define MQ135_PIN 35
#define PIR_PIN 16

// Definir los pines del módulo HX711
#define LOADCELL_DOUT_PIN 33
#define LOADCELL_SCK_PIN 32

// Crear una instancia del HX711
HX711 scale;

#define SOIL_SENSOR_PIN 15  // Pin analógico para el YL-100
#define ONE_WIRE_BUS 19 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//----------------------------------

// URL
const char* serverUrl = "https://070d-190-153-84-219.ngrok-free.app/api/v1/save"; // Replace with your API URL

void setup() {
  Serial.begin(115200);

  // Iniciar el sensor DHT
  dht.begin();

  // Iniciar la comunicación I2C
  Wire.begin(21, 22); // BH1750
  // Iniciar el sensor BH1750
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("Error al iniciar el sensor BH1750");
    while (1);
  }
  WiFi.begin(ssid, password);
  // Configuración de tiempo
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  // Configuración de pines
  pinMode(MQ135_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  
  // Inicializar la celda de carga
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  float calibration_factor = -7050;  // Ajustar después de calibrar la celda de carga
  scale.set_scale(calibration_factor);
  scale.tare(); // Tarar la báscula
  sensors.begin(); // Iniciar sensores de temperatura
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado a Wi-Fi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    //------ FUNCIONES ------
    leerDHT22();
    leerBH1750();
    leerMPXV7002DP();
    leerMQ135();
    leerHC_SR501();
    leerPeso();
    leerHumedadSuelo();
    leerTemperatura();

    // Obtener tiempo
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", &timeinfo);

    // Obtener MAC y usarla como UUID
    String uuidStr = WiFi.macAddress();
    uuidStr.replace(":", "");  // Eliminar los dos puntos

    // Crear JsonObject
    StaticJsonDocument<200> doc;
    doc["event"] ="save";
    doc["data"]["temperature_global"] = String(temperature_C,2);
    doc["data"]["temperature_local"] = String(temperaturaC,2);
    doc["data"]["humidity_global"] = String(humidity_C,2);
    doc["data"]["humidity_local"] = String(humedadPorcentaje,2);
    doc["data"]["movement"] = mov;
    doc["data"]["air_flow"] = String(voltageMQ,2); // ppm
    doc["data"]["weight"] = String(weight,2); // gr
    doc["data"]["light_intensity"] = String(lux,2);
    // Añadir tiempo y UUID
    doc["date_time"] = dateTime;
    doc["client_id"] = "1";
    doc["uuid"] = uuidStr;
    
    // Serializar JsonObject en JSON
    String jsonData;
    serializeJson(doc, jsonData);
    Serial.print(jsonData);
    // Iniciar la solicitud HTTP
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Enviar la solicitud POST con los datos JSON
    int httpResponseCode = http.POST(jsonData);

    // Comprobar la respuesta del servidor
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Respuesta del servidor: " + response);
    } else {
      Serial.println("Error al enviar la solicitud POST");
    }

    // Terminar la conexión
    http.end();
  } else {
    Serial.println("Conexión WiFi perdida");
  }

  delay(300000); // Espera 
}

// Funciones sensores
void leerDHT22() {
  humidity_C = dht.readHumidity();
  temperature_C = dht.readTemperature(); // °C
  if (isnan(humidity_C) || isnan(temperature_C)) {
    return;
  }
}

void leerBH1750() {
  lux = lightMeter.readLightLevel();
  if (lux < 0) {
    return;
  }
}

void leerMPXV7002DP() {
  int sensorValue = analogRead(MPXV7002DP_PIN);
  float voltage = sensorValue * (3.3 / 4095.0);
  pressure = (voltage - 0.5) / 0.2;
}

void leerMQ135() {
  int sensorValue = analogRead(MQ135_PIN);
  voltageMQ = sensorValue * (3.3 / 4095.0);
}

void leerHC_SR501() {
  int pirState = digitalRead(PIR_PIN);
  mov = pirState == HIGH;
}

void leerPeso() {
  if (scale.is_ready()) {
    weight = scale.get_units(10);
  }
}

void leerHumedadSuelo() {
  int soilValue = analogRead(SOIL_SENSOR_PIN);
  humedadPorcentaje = map(soilValue, 0, 4095, 0, 100);
}

void leerTemperatura() {
  sensors.requestTemperatures();
  temperaturaC = sensors.getTempCByIndex(0);
}
