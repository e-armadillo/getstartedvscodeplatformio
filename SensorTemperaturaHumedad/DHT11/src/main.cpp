#include <Arduino.h>
#include <DHT.h>
#include <HardwareSerial.h>

#define DHTPIN 22
DHT dht(DHTPIN, DHT11);


void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float t = dht.readTemperature();
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C");
  delay(2000);
}