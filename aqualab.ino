esphome:
  name: aqualab2
  friendly_name: aquaLab2

esp8266:
  board: nodemcuv2

# Enable logging
logger:
  level: DEBUG

# Enable Home Assistant API
api:
  encryption:
    key: "Xeo4VrpBjXwMOAm0IZE+V+nb8a1qbAv06Tw0HHnVXeQ="

ota:
  - platform: esphome
    password: "aaa5f02765a82de7428bcd3fd1f86fdd"

wifi:
  ssid: FunBox2-4493_24_EXT
  password: !secret wifi_password
  min_auth_mode: WPA2
    # Optional manual IP
  # manual_ip:
  #   static_ip: 192.168.1.99
  #   gateway: 192.168.1.1
  #   subnet: 255.255.255.0

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Aqualab2 Fallback Hotspot"
    password: "IntptWwp2QE8"

captive_portal:
web_server:
  port: 80
  

font:
  - file: 'slkscr.ttf'
    id: font1
    size: 8

i2c:
  sda: GPIO5 # To jest fizyczny pin D1 na NodeMCU
  scl: GPIO4 # To jest fizyczny pin D2 na NodeMCU
  scan: true

one_wire:
  - platform: gpio
    pin: GPIO14 # Przenieś kabel z D4 (GPIO2) na pin D5 (GPIO14)

  
sensor:
  - platform: dallas_temp
    id: temp_wody # Dodane ID
    name: "Temperatura Akwarium"
    index: 0
    update_interval: 30s

display:
  - platform: ssd1306_i2c
    model: "SSD1306 128x32"
    address: 0x3C
    lambda: |-
      it.printf(64, 0, id(font1), TextAlign::TOP_CENTER, "Test wyswietlacza");
      if (id(temp_wody).has_state()) {
        it.printf(64, 15, id(font1), TextAlign::TOP_CENTER, "Temp: %.1f C", id(temp_wody).state);
      } else {
        it.print(64, 15, id(font1), TextAlign::TOP_CENTER, "Sensor błąd...");
      }
