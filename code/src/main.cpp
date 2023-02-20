#include <Arduino.h>
#include <WiFiManager.h> 
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>


//configure Pins and MQTT Topics
const int ArraySize = 7;
const int ledPin = 23;
const int pinArray[ArraySize] = {32, 21, 18, 33, 25, 19, 22};
String topicArray[ArraySize];
const char* mqttId = "FabLux";
String mqttTopic = "zigbee2mqtt/";

//MQTT Setup
WiFiClient espClient;
PubSubClient mqtt(espClient);



//init Json 
StaticJsonDocument<256> doc;
char name[32] = "";

WiFiManager wifiManager;
WiFiManagerParameter custom_topic_0(("topicforpin" + pinArray[0]), ("topic for pin" + pinArray[0]), "Raum_Rechts", 40);
WiFiManagerParameter custom_topic_1(("topicforpin" + pinArray[1]), ("topic for pin" + pinArray[1]), "Raum_Links", 40);
WiFiManagerParameter custom_topic_2(("topicforpin" + pinArray[2]), ("topic for pin" + pinArray[2]), "Fenster_Rechts", 40);
WiFiManagerParameter custom_topic_3(("topicforpin" + pinArray[3]), ("topic for pin" + pinArray[3]), "Fenster_Links", 40);
WiFiManagerParameter custom_topic_4(("topicforpin" + pinArray[4]), ("topic for pin" + pinArray[4]), "Fenster_Mitte", 40);
WiFiManagerParameter custom_topic_5(("topicforpin" + pinArray[5]), ("topic for pin" + pinArray[5]), "Raum_Mitte", 40);
WiFiManagerParameter custom_topic_6(("topicforpin" + pinArray[6]), ("topic for pin" + pinArray[6]), "E_Ecke", 40);
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "10.100.4.3", 40);
WiFiManagerParameter custom_mqtt_user("User", "mqtt User", "fablab", 40);
WiFiManagerParameter custom_mqtt_password("Password", "mqtt Password", "password", 40);

// Setup LED
int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel *pixels;

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String payloadString;
  Serial.print("Message from ");
  Serial.println(topic);
  //json parsing
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  strlcpy(name, doc["state"] | "default", sizeof(name));
  
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  
  Serial.print("Message is ");
  Serial.println(name);
  for (int i=0;i<ArraySize;i++){
    if (strcmp((mqttTopic + topicArray[i]).c_str(), topic) == 0){
      if (strcmp(name, "ON") == 0){
        pixels->setPixelColor(i, pixels->Color(255,255,255));
        pixels->show();
      }
      else if (strcmp(name, "OFF") == 0) {
        pixels->setPixelColor(i, pixels->Color(0,0,0));
        pixels->show();
      }
    }      
  }
} 

void reset(){
  //reset routine
  int timeCount = 0;
  while (!digitalRead(pinArray[0]) && !digitalRead(pinArray[1]) && !digitalRead(pinArray[2]) && !digitalRead(pinArray[3])){
    Serial.print("Reset Routine");
    if(timeCount < 10){
      delay(1000);
      timeCount++;
      Serial.print(".");
    }
    else {
      Serial.println("");
      wifiManager.resetSettings();
      delay(2000);
      Serial.println("");
      Serial.print("Device Reset! Goodbye.");
      ESP.restart();
    }
  }
}

void reconnect(){
  //init custom mqtt configs
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_password);
  wifiManager.addParameter(&custom_mqtt_server);
  //init custom topics for wifimanager
  wifiManager.addParameter(&custom_topic_0);
  wifiManager.addParameter(&custom_topic_1);
  wifiManager.addParameter(&custom_topic_2);
  wifiManager.addParameter(&custom_topic_3);
  wifiManager.addParameter(&custom_topic_4);
  wifiManager.addParameter(&custom_topic_5);
  wifiManager.addParameter(&custom_topic_6);
  
  //wifi connect
  if(!wifiManager.autoConnect("fablux")) {
        Serial.println("Failed to connect");
        //test if reset buttons are pressed
        if(!digitalRead(pinArray[0]) && !digitalRead(pinArray[1]) && !digitalRead(pinArray[2]) && !digitalRead(pinArray[3]) && digitalRead(pinArray[4]) && digitalRead(pinArray[5]) && digitalRead(pinArray[6])){
          reset();
        }
        ESP.restart();
  } 
  else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected... :)");
        //get config
        const char* mqttUser = custom_mqtt_user.getValue();
        const char* mqttPass = custom_mqtt_password.getValue();
        const char* mqtt_ip =  custom_mqtt_server.getValue();    
        //get topics
        topicArray[0] = custom_topic_0.getValue();
        topicArray[1] = custom_topic_1.getValue();
        topicArray[2] = custom_topic_2.getValue();
        topicArray[3] = custom_topic_3.getValue();
        topicArray[4] = custom_topic_4.getValue();
        topicArray[5] = custom_topic_5.getValue();
        topicArray[6] = custom_topic_6.getValue();      
        //setup mqtt
        Serial.print(mqtt_ip);
        mqtt.setServer(mqtt_ip, 1883);
        mqtt.setCallback(mqttCallback);

        //subscribe to all devices topic
        Serial.println("Connecting to MQTT broker");
        Serial.println(mqtt_ip); 
        while(!mqtt.connect(mqttId, mqttUser, mqttPass)){
            Serial.print(".");
            delay(500);
        }
        for (int i=0;i<ArraySize;i++){ 
          mqtt.subscribe((mqttTopic + topicArray[i]).c_str());
        }
          Serial.print("Configured MQTT");
  }
}

void setup() {  
  //init IO Pins
  for (int i=0;i<ArraySize;i++) 
    pinMode(pinArray[i], INPUT_PULLUP);

  //init serial
  Serial.begin(115200);

  //init Neopixel
  pixels = new Adafruit_NeoPixel(ArraySize, ledPin, pixelFormat);
  pixels->begin();
  pixels->clear();
  Serial.print("Configured NeoPixel");

  //init wifi
  WiFiManager wifiManager;
  
  //start wifi config wizard  
  reconnect();
}

void loop() {
  for(int i=0;i < ArraySize; i++) {
    
    if (!digitalRead(pinArray[i])){
      Serial.println("Button " + topicArray[i] + " got pressed");
      mqtt.publish( (mqttTopic + topicArray[i] + "/set/state").c_str(), "TOGGLE");
      while (!digitalRead(pinArray[i])){
        //debounce button
        delay(20);
        //test if reset buttons are pressed
        if(!digitalRead(pinArray[0]) && !digitalRead(pinArray[1]) && !digitalRead(pinArray[2]) && !digitalRead(pinArray[3]) && digitalRead(pinArray[4]) && digitalRead(pinArray[5]) && digitalRead(pinArray[6])){
          reset();
        }
      }
    }
  }
  delay(20);
  mqtt.loop();
  wifiManager.process();

  //check if wifi is connected
  if(!mqtt.connected()){
    Serial.println("Lost Mqtt Connection, reconnect");
    reconnect();
  }
}