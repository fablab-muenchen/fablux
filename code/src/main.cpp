#include <Arduino.h>
#include <WiFiManager.h> 
#include <PubSubClient.h>
#include <iostream>
#include <PololuLedStrip.h>


bool status;

const int pinArray[7] = {32, 19, 21, 18, 25, 33, 22};
const String topicArray[7] ={"Raum_Rechts", "Raum_Mitte", "Raum_Links", "Fenster_Rechts", "Fenster_Mitte", "Fenster_Links", "E_Ecke"};
const int ArraySize = sizeof(pinArray)/sizeof(pinArray[0]);
const IPAddress mqtt_server(127, 0, 0, 1);
const int ledPin = 23;






//MQTT Setup
WiFiClient espClient;
PubSubClient mqtt(espClient);

// Setup LED
  PololuLedStrip<ledPin> ledStrip;
  rgb_color colors[ArraySize];

void setup() {  
  //init IO Pins
    for (int i=0;i<ArraySize;i++) 
      pinMode(pinArray[i], INPUT_PULLUP);


  
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
        Serial.println("connected...)");
        Serial.println("The Ip address is:" );
        mqtt.setServer(mqtt_server, 1883);
        mqtt.setCallback(mqttCallback);



  }
}

void loop() {
  
  for(int i=0;i < ArraySize; i++) {
    if (!digitalRead(pinArray[i]))
    mqtt.publish( ("zigbee2mqtt/" + topicArray[i] + "/set/state").c_str(), "TOGGLE");
  }
  mqtt.loop();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String stringTopic = topic;
      for (int i=0;i<ArraySize;i++){
        if (String loopTopic = "zigbee2mqtt/" + topicArray[i]; stringTopic == loopTopic);
          if (String s(payload, sizeof(payload)); s == "ON")
          {
            colors[i] = rgb_color(255, 255, 255);
          }
          else if (String s(payload, sizeof(payload)); s == "OFF")
          {
            colors[i] = rgb_color(0, 0, 0);
          }
      }


} 