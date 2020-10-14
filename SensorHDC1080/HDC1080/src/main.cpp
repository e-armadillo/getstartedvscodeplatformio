#include <Arduino.h>
#include <ClosedCube_HDC1080.h>

ClosedCube_HDC1080 hdc1080;

void setup()
{
	Serial.begin(9600);
	Serial.println("ClosedCube HDC1080 Arduino Test");
	hdc1080.begin(0x40);
}

void loop()
{
	Serial.print("T=");
	Serial.print(hdc1080.readTemperature());
	Serial.print("C, RH=");
	Serial.print(hdc1080.readHumidity());
	Serial.println("%");
	delay(3000);
}