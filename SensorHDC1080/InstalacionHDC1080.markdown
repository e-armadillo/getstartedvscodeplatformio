# Instalacion sensor de temperatura/humedad HDC1080

Es un sensor de temperatura y humedad de mayor precision que puede ser usada mediante un conector **grove** o poder soldarle pines para utilizarlo en protoboard.

![](Sensorhdc1080.png)
<center>
*Entrada blanca para conector Grove y orificios a los costados para soldar pines para protoboard*
</center>

***

## Conexion del sensor

Este sensor consta de los siguientes puertos:
- VCC
- GND
- SDA
- SCL

Donde VCC se conecta a 5V (en el caso del esp8266 se sugiere conectar al pin 3V3). El puerto GND al GND del microcontrolador.

Los puertos SDA y SCL (comunicacion I2C) son respectivamente la linea de datos y la linea de reloj(No profundizare en esto) y van conectados a los pines correspondientes del esp8266.

![](Esp8266.png)

- SCL corresponde al pin D1 (GPIO5)
- SDA corresponde al pin D2 (GPIO4)

***

## Programacion del Sensor

Utilizando PlatformIO en Visual Studio, creamos nuestro proyecto y le añadimos la libreria **ClosedCubeHDC1080** la cual nos permitira usar los comandos del sensor.

```cpp
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
```

El sensor si o si debe ir conectado a los pines SDA y SCL del microcontrolador por lo que en la programacion no es necesario definir un puerto de salida o entrada, estos ya estan establecidos.
Podemos observar los resultados en el Monitor Serie.

***

## Sensor HDC1080 y MQTT

El codigo para poder comunicar el sensor mediante MQTT a un Broker es el siguiente:

```cpp
/*****************************************
 * Include Libraries
 ****************************************/

#include <Arduino.h>
#include <PubSubClient.h>

#include <DNSServer.h>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

/*****************************************
 * Include Senspr Humedity
 ****************************************/
#include <ClosedCube_HDC1080.h>

ClosedCube_HDC1080 hdc1080;

//Constant to connect to the MQTT broker
const char *mqtt_address = "192.168.0.10";
int mqtt_port = 1883;
//Constant to login
const char *mqtt_user = "esp32";
const char *mqtt_pass = "esp32";

//To choose topic
const char *subscribe = "/home";
const char *publish = "";

//To connect to wifi
const char* wifi_ssid = "VTR-4751327";
const char* password = "Cb8mffrcmQzq";


char topic[150];
char payload[50];
String clientMac = "";
unsigned char mac[6];

struct Config {
   /* data */
  char name[20];
  bool enabled;
  int hour;
} config;


/****************************************
 * Initialize a global instance
 ****************************************/
WiFiClient espClient;
PubSubClient client(espClient);

char msg_c[60];

//wifi connection
void W_setup(){
      delay(10);
      Serial.println();
      Serial.print("Connecting to SSID: ");
      Serial.println(wifi_ssid);

      WiFi.begin(wifi_ssid,password);

      while (WiFi.status() != WL_CONNECTED){
        delay(750);
        Serial.print("#");
      }
      
      Serial.println("");
      Serial.println("Connecting to WiFi network");
      Serial.println("IP Address: ");
      Serial.println(WiFi.localIP());
}

//this function is called when a new message arrive at the client
void callback(char* topic, byte* payload, unsigned int  long length){
      String msg_in = "";
      Serial.print(topic);
      Serial.println("");
      for( unsigned int long i=0; i<length; i++){
        msg_in += (char)payload[i];
      }
      msg_in.trim();
      Serial.println("Message:" + msg_in);
}

void setup() {
  //Setup of Wifi
  Serial.begin(9600);
  W_setup();

  //Configuration for Client
  client.setServer(mqtt_address,mqtt_port);
  client.setCallback(callback);

  hdc1080.begin(0x40);
}
//Reconnect function
void reconnect(){
      while(!client.connected()){
        Serial.print("Connecting to MQTT");
        String Id = ""; //Choose your Id
        Id += String(random(0xffff),HEX);
        //The line above helps us to change the id if
        //there is another client with the same, just
        //adding a number next to it, we use 0xffff HEX = 65535 Dec

        if (client.connect(Id.c_str(),mqtt_user,mqtt_pass)){
          Serial.println("Connected");
            if(client.subscribe(subscribe)){
              Serial.println("Subscription Established");
            } else {
              Serial.println("Subscription Failed");
            }
          } else {
            Serial.print("Failed");
            Serial.print(client.state());
            Serial.println("Trying to reconnecting..");
            delay(3000);
          }
      }

}
//Here we put what we want to do..
void loop() {
  if (client.connected()){
    //What we want to send
    //Example

    float temperature = hdc1080.readTemperature();

    String str = String(temperature);
    str.toCharArray(msg_c,25);
    client.publish(subscribe,msg_c);
    delay(1000);
  }

  else {
    //In the opposite case we call the Reconnect function.
    reconnect();

  }
}
```