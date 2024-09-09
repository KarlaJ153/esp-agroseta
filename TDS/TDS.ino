#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A1
GravityTDS gravityTds;

float temperature = 25.0; // Temperatura predeterminada, puedes usar un sensor de temperatura para obtener esta lectura
float tdsValue = 0.0;

void setup() {
  Serial.begin(9600);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  // Voltaje de referencia en el ADC, predeterminado a 5.0V en Arduino UNO
  gravityTds.setAdcRange(1024);  // Rango ADC de 10 bits, Arduino UNO / Arduino Mega
  gravityTds.begin();  // Inicialización
}

void loop() {
  gravityTds.setTemperature(temperature);  // Establecer la temperatura y ejecutar la compensación de temperatura
  gravityTds.update();  // Muestra y calcula
  tdsValue = gravityTds.getTdsValue();  // Obtener el valor de TDS
  
  Serial.print("TDS Value: ");
  Serial.print(tdsValue, 0);  // Imprimir el valor de TDS sin decimales
  Serial.println(" ppm");
  
  delay(1000); // Esperar 1 segundo antes de la siguiente lectura
}
