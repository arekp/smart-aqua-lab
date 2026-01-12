esphome:
  name: monitor-wody
  friendly_name: Monitor pH i Temperatury

esp32:
  board: esp32dev

# Konfiguracja wyświetlacza I2C
i2c:
  sda: 21
  scl: 22
  scan: true

# Obsługa czujnika temperatury DS18B20
one_wire:
  - platform: gpio
    pin: 14

sensor:
  # Temperatura
  - platform: dallas_temp
    address: 0xXXXXXXXXXXXXXXXX # Podaj adres swojego czujnika po wgraniu i logach
    name: "Temperatura Wody"
    id: temp_wody
    update_interval: 15s

  # pH (Czujnik analogowy PH-4502C)
  - platform: adc
    pin: 34
    name: "pH Odczyt Napięcia"
    id: ph_voltage
    update_interval: 15s
    unit_of_measurement: "V"
    accuracy_decimals: 2
    # Kalibracja: musisz sprawdzić napięcie dla pH 4.0 i pH 7.0
    filters:
      - calibrate_linear:
          - 2.5 -> 7.0   # Przykład: 2.5V to pH 7.0
          - 3.0 -> 4.0   # Przykład: 3.0V to pH 4.0
    on_value:
      then:
        - component.update: oled_display

# Konfiguracja wyświetlacza OLED
display:
  - platform: ssd1306_i2c
    model: "SSD1306 128x32" # Model 0.91'' to zazwyczaj 128x32
    id: oled_display
    address: 0x3C
    lambda: |-
      it.print(0, 0, id(font_small), "MONITOR WODY");
      it.printf(0, 15, id(font_main), "pH: %.2f", id(ph_voltage).state);
      it.printf(0, 30, id(font_main), "Temp: %.1f C", id(temp_wody).state);

# Czcionki potrzebne do wyświetlacza
font:
  - file: "gfonts://Roboto"
    id: font_small
    size: 8
  - file: "gfonts://Roboto"
    id: font_main
    size: 12
