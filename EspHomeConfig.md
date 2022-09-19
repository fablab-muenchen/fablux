# ESP Home Config
old config from homeassistand


```c
esphome:
  name: fablux

esp32:
  board: nodemcu-32s
  framework:
    type: arduino

// Enable logging
logger:

// Enable Home Assistant API
api:
  encryption:
    key: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

ota:
  password: "XXXXXXXXXXXXXXXXXXXXXXXXXX"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  // Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Fablux Fallback Hotspot"
    password: "XXXXXXXXXX"

captive_portal:

mqtt:
  broker: XXXXXXXXXXXXXX
  username: !secret mqtt_user
  password: !secret mqtt_password
  id: mqtt_client
  on_json_message:
    - topic: zigbee2mqtt/E_Ecke
      then:
        - light.addressable_set:
            id: led
            range_from: 6
            range_to: 6
            green: !lambda |-
              if (strcmp(x["state"],"ON")==0){
                ESP_LOGD("E-Ecke LED", "ON");
                return 1.0;
              }else{
                ESP_LOGD("E-Ecke LED", "OFF");
                return 0.0;
              }
            red: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;
            blue: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;

    - topic: zigbee2mqtt/Raum_Rechts
      then:
        - light.addressable_set:
            id: led
            range_from: 0
            range_to: 0
            green: !lambda |-
              if (strcmp(x["state"],"ON")==0){
                ESP_LOGD("Raum Rechts LED", "ON");
                return 1.0;
              }else{
                ESP_LOGD("Raum Rechts LED", "OFF");
                return 0.0;
              }
            red: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;
            blue: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;

    - topic: zigbee2mqtt/Raum_Mitte
      then:
        - light.addressable_set:
            id: led
            range_from: 1
            range_to: 1
            green: !lambda |-
              if (strcmp(x["state"],"ON")==0){
                ESP_LOGD("Raum Mitte LED", "ON");
                return 1.0;
              }else{
                ESP_LOGD("Raum Mitte LED", "OFF");
                return 0.0;
              }
            red: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;
            blue: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;

    - topic: zigbee2mqtt/Raum_Links
      then:
        - light.addressable_set:
            id: led
            range_from: 2
            range_to: 2
            green: !lambda |-
              if (strcmp(x["state"],"ON")==0){
                ESP_LOGD("Raum Links LED", "ON");
                return 1.0;
              }else{
                ESP_LOGD("Raum Links LED", "OFF");
                return 0.0;
              }
            red: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;
            blue: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;

    - topic: zigbee2mqtt/Fenster_Rechts
      then:
        - light.addressable_set:
            id: led
            range_from: 3
            range_to: 3
            green: !lambda |-
              if (strcmp(x["state"],"ON")==0){
                ESP_LOGD("Fenster Rechts LED", "ON");
                return 1.0;
              }else{
                ESP_LOGD("Fenster Rechts LED", "OFF");
                return 0.0;
              }
            red: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;
            blue: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;

    - topic: zigbee2mqtt/Fenster_Mitte
      then:
        - light.addressable_set:
            id: led
            range_from: 4
            range_to: 4
            green: !lambda |-
              if (strcmp(x["state"],"ON")==0){
                ESP_LOGD("Fenster Mitte LED", "ON");
                return 1.0;
              }else{
                ESP_LOGD("Fenster Mitte LED", "OFF");
                return 0.0;
              }
            red: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;
            blue: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;

    - topic: zigbee2mqtt/Fenster_Links
      then:
        - light.addressable_set:
            id: led
            range_from: 5
            range_to: 5
            green: !lambda |-
              if (strcmp(x["state"],"ON")==0){
                ESP_LOGD("Fenster Links LED", "ON");
                return 1.0;
              }else{
                ESP_LOGD("Fenster Links LED", "OFF");
                return 0.0;
              }
            red: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;
            blue: !lambda |-
              if (strcmp(x["state"],"ON")==0) return 1.0;
              else return 0.0;




light:
  - platform: fastled_clockless
    chipset: WS2812B
    pin: 23
    num_leds: 7
    rgb_order: RGB
    id: led
    name: "FastLED Light"

binary_sensor:
  - platform: gpio
    name: "E-Ecke"
    pin:
      number: 22
      mode:
        input: true
        pullup: true
      inverted: true
    on_click:
      min_length: 50ms
      max_length: 5000ms
      then:
        - logger.log: 'click registered'
        - mqtt.publish:
            topic: 'zigbee2mqtt/E_Ecke/set/state'
            payload: 'TOGGLE'
  - platform: gpio
    name: "Raum Rechts"
    pin:
      number: 32
      mode:
        input: true
        pullup: true
      inverted: true
    on_click:
      min_length: 50ms
      max_length: 5000ms
      then:
        - logger.log: 'click registered'
        - mqtt.publish:
            topic: 'zigbee2mqtt/Raum_Rechts/set/state'
            payload: 'TOGGLE'
  - platform: gpio
    name: "Raum Mitte"
    pin:
      number: 19
      mode:
        input: true
        pullup: true
      inverted: true
    on_click:
      min_length: 50ms
      max_length: 5000ms
      then:
        - logger.log: 'click registered'
        - mqtt.publish:
            topic: 'zigbee2mqtt/Raum_Mitte/set/state'
            payload: 'TOGGLE'
  - platform: gpio
    name: "Raum Links"
    pin:
      number: 21
      mode:
        input: true
        pullup: true
      inverted: true
    on_click:
      min_length: 50ms
      max_length: 5000ms
      then:
        - logger.log: 'click registered'
        - mqtt.publish:
            topic: 'zigbee2mqtt/Raum_Links/set/state'
            payload: 'TOGGLE'
  - platform: gpio
    name: "Fenster Rechts"
    pin:
      number: 18
      mode:
        input: true
        pullup: true
      inverted: true
    on_click:
      min_length: 50ms
      max_length: 5000ms
      then:
        - logger.log: 'click registered'
        - mqtt.publish:
            topic: 'zigbee2mqtt/Fenster_Rechts/set/state'
            payload: 'TOGGLE'
  - platform: gpio
    name: "Fenster Mitte"
    pin:
      number: 25
      mode:
        input: true
        pullup: true
      inverted: true
    on_click:
      min_length: 50ms
      max_length: 5000ms
      then:
        - logger.log: 'click registered'
        - mqtt.publish:
            topic: 'zigbee2mqtt/Fenster_Mitte/set/state'
            payload: 'TOGGLE'
  - platform: gpio
    name: "Fenster Links"
    pin:
      number: 33
      mode:
        input: true
        pullup: true
      inverted: true
    on_click:
      min_length: 50ms
      max_length: 5000ms
      then:
        - logger.log: 'click registered'
        - mqtt.publish:
            topic: 'zigbee2mqtt/Fenster_Links/set/state'
            payload: 'TOGGLE'`

```