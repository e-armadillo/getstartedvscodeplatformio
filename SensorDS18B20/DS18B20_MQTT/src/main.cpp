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

#include <DallasTemperature.h>
#include <OneWire.h>

OneWire DXensor_Temperature(2); //Pin 4 del esp8266
DallasTemperature temp(&DXensor_Temperature);


//Constant to connect to the MQTT broker
const char *mqtt_address = "192.168.0.16";
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

  temp.begin();
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

    temp.requestTemperatures();
    float temperature = temp.getTempCByIndex(0);

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
