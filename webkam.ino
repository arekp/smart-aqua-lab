---
esphome:
  name: esp32-cam-ryby
  friendly_name: esp32_cam_ryby

esp32:
  board: esp32dev
  framework:
    type: esp-idf
  # Usunięto psram: stąd!

psram: # <--- POPRAWNE MIEJSCE: to jest osobny komponent na tym samym poziomie co esp32:
  # Dla klasycznego ESP32-CAM (WROVER) nie trzeba podawać opcji, 
  # domyślny tryb (quad) i tak zostanie użyty, gdy komponent esp32_camera go włączy. 
  # Możesz zostawić pusty blok, aby go włączyć jawnie lub usunąć go całkowicie, 
  # ponieważ esp32_camera_web_server zazwyczaj włącza go automatycznie. 
  # Jednak pozostawienie pustego bloku w tym miejscu jest prawidłowe i jawne:
  
# Enable logging
logger:
  # Można dodać opcjonalnie:
  # level: INFO 
  # hardware_uart: UART0 

# Enable Home Assistant API
api:
  encryption:
    key: XXXXXX

ota:
  - platform: esphome
    password: XXXXX

wifi:
#  ssid: !secret wifi_ssid_pietro
#  password: !secret wifi_password_pietro
  ssid: XXXXXX
  password: XXXXX

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Esp32-Cam-Ryby Fallback Hotspot"
    password: "arekp1234"

captive_portal:

# Definicja magistrali I2C (typowa dla modułu AI-Thinker)
i2c:
  - id: camera_i2c
    sda: GPIO26
    scl: GPIO27
    # Opcjonalnie można dodać:
    # frequency: 400kHz # 400kHz jest często zalecane dla kamer

esp32_camera:
  # Piny dla typowego modułu ESP32-CAM AI-Thinker
  external_clock:
    pin: GPIO0
    frequency: 20MHz
  i2c_id: camera_i2c
  data_pins: [GPIO5, GPIO18, GPIO19, GPIO21, GPIO36, GPIO39, GPIO34, GPIO35]
  vsync_pin: GPIO25
  href_pin: GPIO23
  pixel_clock_pin: GPIO22
  power_down_pin: GPIO32
  
  # Ustawienia dodatkowe dla lepszej stabilności/jakości
  name: "Kamera Ryby" # Nazwa encji w Home Assistant
  resolution: 1024x768 # Przykład, typowe to 800x600 lub 1024x768 (XGA)
  # Opcjonalne korekcje obrazu:
  # horizontal_mirror: True
  # vertical_flip: True

# Wbudowana dioda LED (Flash)
output:
  - platform: gpio
    pin: GPIO4
    id: camera_led_output

light:
  - platform: binary
    name: "Lampa Ryby"
    output: camera_led_output
    # Opcjonalnie możesz dodać "Restore Mode", aby pamiętać stan diody po restarcie
    # restore_mode: ALWAYS_OFF 

# Serwer WWW do podglądu przez przeglądarkę (niezależny od Home Assistant API)
esp32_camera_web_server:
  # Użycie tego komponentu często jest opcjonalne, 
  # ponieważ podgląd jest dostępny przez Home Assistant API.
  - port: 8080
    mode: stream
  - port: 8081
    mode: snapshot    

# Opcjonalnie: sensor mierzący siłę sygnału WiFi
sensor:
  - platform: wifi_signal
    name: "Sygnał WiFi Ryby"
    update_interval: 60s
