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
    id: font_small
    size: 8
  - file: 'Roboto-Bold.ttf'
    id: font_big
    size: 22  # 

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
# Pomiar napięcia z pinu A0 (PH-4502C)
  - platform: adc
    pin: A0
    id: ph_voltage
    name: "PH Napiecie"
    update_interval: 5s
    unit_of_measurement: "V"
    filters:
      - multiply: 3.3 # Kalibracja zakresu ADC dla NodeMCU (0.0-1.0 -> 0.0-3.3V)
    accuracy_decimals: 2

  # Przeliczenie napięcia na pH
  - platform: template
    name: "Wartość pH"
    id: ph_value
    unit_of_measurement: "pH"
    accuracy_decimals: 2
    update_interval: 5s
    lambda: |-
      /* 
         WZÓR KALIBRACJI: pH = 7 + (V_neutral - V_measured) * slope
         Dla PH-4502C zazwyczaj pH 7 to ok. 2.5V (jeśli zasilasz z 5V).
         PONIŻSZY WZÓR JEST PRZYKŁADOWY I WYMAGA KALIBRACJI W BUFORACH (pH 4 i 7).
      */
      float v_neutral = 2.5; // Napięcie dla pH 7.0 (zmierz to miernikiem!)
      return 7.0 + (v_neutral - id(ph_voltage).state) * 3.5;

# Automatyczna zmiana stron na wyświetlaczu co 5 sekund
interval:
  - interval: 5s
    then:
      - display.page.show_next: my_display

display:
  - platform: ssd1306_i2c
    id: my_display
    model: "SSD1306 128x32"
    address: 0x3C
    pages:
      - id: page1
        lambda: |-
          it.printf(0, 0, id(font_small), TextAlign::TOP_LEFT, "TEMP:");
          if (id(temp_wody).has_state()) {
            it.printf(128, 32, id(font_big), TextAlign::BOTTOM_RIGHT, "%.1f C", id(temp_wody).state);
          } else {
            it.print(64, 20, id(font_small), TextAlign::CENTER, "Brak danych");
          }
      - id: page2
        lambda: |-
          it.printf(0, 0, id(font_small), TextAlign::TOP_LEFT, "NAPIECIE A0:");
          it.printf(128, 32, id(font_big), TextAlign::BOTTOM_RIGHT, "%.2f V", id(ph_voltage).state);

      - id: page3
        lambda: |-
          it.printf(0, 0, id(font_small), TextAlign::TOP_LEFT, "ODCZYT PH:");
          if (id(ph_value).has_state()) {
            it.printf(128, 32, id(font_big), TextAlign::BOTTOM_RIGHT, "%.2f", id(ph_value).state);
          } else {
            it.print(64, 20, id(font_small), TextAlign::CENTER, "Kalibracja...");
          }