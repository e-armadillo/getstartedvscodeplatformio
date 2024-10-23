#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire DXensor_Temperature(2); //Pin 4

DallasTemperature temp(&DXensor_Temperature);

void setup() {
  Serial.begin(9600);
  temp.begin();

}

void loop() {

  temp.requestTemperatures();
  float temperatura = temp.getTempCByIndex(0);

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  delay(100);
}