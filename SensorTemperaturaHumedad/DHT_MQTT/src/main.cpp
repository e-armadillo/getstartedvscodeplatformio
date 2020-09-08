  
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <DHT.h>

//Begin dht setup
#define DHTPIN 22
DHT dht(DHTPIN, DHT11);
//Constant to connect to the MQTT broker
const char *mqtt_address = "";
int mqtt_port = 1883;
//Constant to login
const char *mqtt_user = "";
const char *mqtt_pass = "";

//To choose topic
const char *subscribe = "";
const char *publish = "";

//To connect to wifi
const char* wifi_ssid = "";
const char* password = "";

//setting a WifiClient
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
void callback(char* topic, byte* payload, unsigned int length){
      String msg_in = "";
      Serial.print(topic);
      Serial.println("");
      for(int i=0; i<length; i++){
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
  dht.begin();
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
    String str = "Temperature: "+String(temperature())+" C°";
    str.toCharArray(msg_c,25);
    client.publish(subscribe,msg_c);
    delay(1000);
  }

  else {
    //In the opposite case we call the Reconnect function.
    reconnect();

  }
}

int temperature(){

  float t = dht.readTemperature();
  delay(2000);
  return t;
}