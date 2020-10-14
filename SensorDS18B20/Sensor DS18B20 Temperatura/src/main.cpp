#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

<<<<<<< HEAD
OneWire DXensor_Temperature(22);
=======
OneWire DXensor_Temperature(2); //Pin 4
>>>>>>> 1c21aa533cbbde761eecee11972dc5b5d28ebc92

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
<<<<<<< HEAD

=======
>>>>>>> 1c21aa533cbbde761eecee11972dc5b5d28ebc92
}