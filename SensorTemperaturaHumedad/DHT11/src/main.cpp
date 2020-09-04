#include <Arduino.h>
#include <DHT.h>

DHT dht(5, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}