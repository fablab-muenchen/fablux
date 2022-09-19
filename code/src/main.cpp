#include <Arduino.h>
#include <WiFiManager.h> 
bool status
void setup() {
  //init serial
  Serial.begin(115200);
  //init wifi
  WiFiManager wifiManager;
  //start wifi config wizard
  
  status = wifiManager.autoConnect("fablux");
  if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...)");
        Serial.println("The Ip address is:" )

}

void loop() {
  // put your main code here, to run repeatedly:
}