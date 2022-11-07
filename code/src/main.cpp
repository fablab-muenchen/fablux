#include <Arduino.h>
#include <WiFiManager.h> 
#include <PubSubClient.h>
//#include <iostream>
#include <Adafruit_NeoPixel.h>



//configure Pins and MQTT Topics
const int ledPin = 23;
const int pinArray[7] = {32, 19, 21, 18, 25, 33, 22};
const String topicArray[7] ={"Raum_Rechts", "Raum_Mitte", "Raum_Links", "Fenster_Rechts", "Fenster_Mitte", "Fenster_Links", "E_Ecke"};
const IPAddress mqtt_server(00,00,00,00);
#define mqttUser = "XXXXX"
#define mqttPass = "XXXXX"


const int ArraySize = sizeof(pinArray)/sizeof(pinArray[0]);
bool status;

//MQTT Setup
WiFiClient espClient;
PubSubClient mqtt(espClient);

// Setup LED
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(ArraySize, ledPin, NEO_GRB + NEO_KHZ800);


void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String stringTopic = topic;
      for (int i=0;i<ArraySize;i++){
        if (String loopTopic = "zigbee2mqtt/" + topicArray[i]; stringTopic == loopTopic);
          if (String s(payload, sizeof(payload)); s == "ON")
          {
            pixels.setPixelColor(pinArray[i], pixels.Color(255,255,255));
            pixels.show();

          }
      }
} 

void setup() {  
  //init IO Pins
    for (int i=0;i<ArraySize;i++) 
      pinMode(pinArray[i], INPUT_PULLUP);
  pixels.begin();
  //init serial
  Serial.begin(115200);

  //init wifi
  WiFiManager wifiManager;
  
  //start wifi config wizard  
  status = wifiManager.autoConnect("fablux");
  if(!status) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
  else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected... :)");
        mqtt.setServer(mqtt_server, 1883);
        mqtt.setCallback(mqttCallback);
  }
}

void loop() {
  
  for(int i=0;i < ArraySize; i++) {
    if (!digitalRead(pinArray[i])){
      Serial.println("Button " + topicArray[i] + " got pressed");
      mqtt.publish( ("zigbee2mqtt/" + topicArray[i] + "/set/state").c_str(), "TOGGLE");
      while (!digitalRead(pinArray[i])){
        delay(10);
      }
      
    }
  }
  delay(10);
  mqtt.loop();
}