# Smart-Aqua-Lab

**Kompleksowy system inteligentnego monitorowania i zarządzania parametrami akwarium.**

Smart-Aqua-Lab to otwartoźródłowe rozwiązanie IoT zaprojektowane do precyzyjnego nadzoru nad ekosystemem wodnym. System łączy pomiary fizykochemiczne w czasie rzeczywistym z zaawansowaną analizą wizyjną opartą na sztucznej inteligencji.

---

## 🏗 Architektura Systemu

System opiera się na architekturze rozproszonej podzielonej na trzy warstwy:

1.  **Warstwa Percepcyjna (Hardware):** Mikrokontrolery ESP8266 i ESP32-CAM zbierające dane z sensorów (pH, TDS, temperatura, obraz).
2.  **Warstwa Komunikacyjna:** Protokół ESPHome oraz MQTT zapewniający niskoopóźnieniową transmisję danych do jednostki centralnej.
3.  **Warstwa Analityczno-Prezentacyjna:** Serwer Home Assistant, baza danych InfluxDB oraz modele AI (TensorFlow) do analizy zachowań ryb.

---

## 🛠 Specyfikacja Techniczna

### 1. Komponenty Sprzętowe
| Komponent | Model | Rola |
| :--- | :--- | :--- |
| **Kontroler Sensorów** | ESP8266 (NodeMCU v2) | Obsługa pH, temperatury i wyświetlacza |
| **Moduł Wizyjny** | ESP32-CAM (AI-Thinker) | Streaming wideo i detekcja anomalii |
| **Czujnik pH** | PH-4502C + sonda E-201C | Pomiar kwasowości wody |
| **Czujnik Temp.** | DS18B20 (Waterproof) | Pomiar temperatury (OneWire) |
| **Wyświetlacz** | OLED SSD1306 (128x32) | Lokalny podgląd parametrów |
| **Czujnik Jakości** | TDS Meter v1.0 | Pomiar czystości wody (https://pl.aliexpress.com/item/1005009863408748.html?spm=a2g0o.order_list.order_list_main.5.21ef1c24wieUSz&gatewayAdapt=glo2pol)[link] |
| **Zasilanie** | Stabilizowane 5V DC | Zasilanie modułów pH i ESP |
| **Czujnik światła (PAR/Lux)**| Dla akwariów roślinnych lub morskich. Pozwala sprawdzić, czy diody LED nie tracą mocy z czasem.||
| **Pomiar CO2 (Drop Checker AI)** | Zamiast drogich sond CO2, możesz skierować kamerę na szklany indykator (drop checker) i za pomocą OpenCV analizować jego kolor (niebieski/zielony/żółty).||

### 2. Stack Technologiczny
*   **Embedded:** ESPHome, Arduino Framework, FreeRTOS.
*   **Backend:** Python (FastAPI), Mosquitto MQTT.
*   **Data Science:** Pandas, TensorFlow, OpenCV.
*   **DevOps:** Docker, Docker Compose, Prometheus, Grafana.

---

## ⚙️ Konfiguracja i Implementacja (ESPHome)

### Moduł Sensorów (ESP8266)
Główny moduł pomiarowy wykorzystuje magistralę I2C dla wyświetlacza oraz 1-Wire dla temperatury.

**Kluczowe przypisania PIN-ów:**
*   **I2C (OLED):** SDA -> GPIO5 (D1), SCL -> GPIO4 (D2)
*   **1-Wire (Temp):** GPIO14 (D5)
*   **Analog pH:** A0 (wejście napięciowe)

**Algorytm przeliczania pH:**
```cpp
// Wzór implementowany w ESPHome
pH = 7.0 + (v_neutral - v_measured) * slope
```

### Moduł Wizyjny (ESP32-CAM)
Konfiguracja obejmuje serwer strumieniowy oraz integrację z Home Assistant API. Obsługuje on rozdzielczość do 1024x768 (XGA) i posiada wbudowane sterowanie oświetleniem (Flash LED).

---

## ⚖️ Kalibracja Czujnika pH (PH-4502C)

Prawidłowa kalibracja jest kluczowa dla dokładności systemu.

1.  **Przygotowanie:** Zasil moduł PH-4502C stabilnym napięciem 5V. Połącz GND modułu z GND ESP8266.
2.  **Kalibracja Punktu Neutralnego (pH 7.0):**
    *   Zanurz sondę w roztworze buforowym pH 7.0.
    *   Odczytaj napięcie na porcie A0 (widoczne na ekranie OLED w trybie serwisowym).
    *   Kręć potencjometrem (bliżej złącza BNC), aż uzyskasz wartość ~2.5V lub zaktualizuj zmienną `v_neutral` w kodzie.
3.  **Kalibracja Nachylenia (Slope):**
    *   Zanurz sondę w roztworze pH 4.0.
    *   Dostosuj mnożnik (standardowo 3.5) w sekcji `lambda` konfiguracji, aby odczyt był zgodny z roztworem.

### 1. Czujnik Światła (Lux/PAR) na ESP8266

Zamiast budować drogi luksomierz, użyjemy czujnika **BH1750**. Jest on bardzo precyzyjny, tani i komunikuje się przez I2C, co oznacza, że możesz go podłączyć do tych samych pinów, co Twój wyświetlacz OLED.

#### Sprzęt:
*   **Czujnik:** BH1750 (nazywany też GY-302).
*   **Podłączenie:** SDA i SCL do tych samych pinów co OLED (GPIO4 i GPIO5). ESP8266 obsłuży oba urządzenia na jednej magistrali.

#### Implementacja w ESPHome:
Dodaj poniższy blok do swojego pliku `.yaml`:

```yaml
sensor:
  - platform: bh1750
    name: "Intensywność światła"
    address: 0x23
    update_interval: 60s
    unit_of_measurement: "lx"
    id: lux_meter

  # Przelicznik na PAR (uproszczony dla LED)
  - platform: template
    name: "Szacowane PAR"
    unit_of_measurement: "µmol/m²/s"
    lambda: |-
      return id(lux_meter).state / 54.0; 
```
*Uwaga: Przelicznik `54.0` jest orientacyjny dla białych diod LED (tzw. LER - Luminous Efficacy Radiation). Pozwala on oszacować, ile światła faktycznie dociera do roślin.*

#### Zastosowanie:
*   **Monitoring zużycia LED:** Zapisuj maksymalne natężenie światła w południe. Jeśli po roku spadnie o 15%, system powiadomi Cię o konieczności wymiany diod.
*   **Długość dnia:** W Home Assistant możesz stworzyć wykres "Light Hours", który pokaże, czy czas świecenia jest stabilny.

---

### 2. Monitorowanie CO2 (Drop Checker AI)

To najbardziej innowacyjna część. Drop Checker to szklany pojemnik z płynem (reagentem), który zmienia kolor:
*   **Niebieski:** Za mało CO2.
*   **Zielony:** Optymalnie (~30 mg/l).
*   **Żółty:** Za dużo (niebezpieczne dla ryb!).

#### Jak to zrealizować?
Nie potrzebujemy sensora CO2 za 500 zł. Wykorzystamy Twoją **ESP32-CAM**.

**Krok 1: Umiejscowienie**
Zamocuj Drop Checker w akwarium tak, aby znajdował się w polu widzenia kamery (najlepiej w rogu, blisko szyby).

**Krok 2: Analiza obrazu (OpenCV w Pythonie)**
Ponieważ ESP32-CAM jest za słabe na pełne OpenCV, analizę wykonamy na serwerze (tam, gdzie masz FastAPI lub Home Assistant).

**Logika algorytmu:**
1.  Wytnij mały fragment obrazu (ROI - Region of Interest), gdzie znajduje się Drop Checker.
2.  Przekonwertuj obraz z formatu RGB na **HSV** (Hue, Saturation, Value). Kolor w HSV jest znacznie łatwiejszy do zidentyfikowania niż w RGB (gdzie zmiana oświetlenia psuje wyniki).
3.  Zlicz dominujący odcień (Hue):
    *   Hue 160-240: Niebieski.
    *   Hue 80-140: Zielony.
    *   Hue 30-70: Żółty.

**Przykładowy kod Python (do backendu):**
```python
import cv2
import numpy as np

def analyze_co2(image_path):
    img = cv2.imread(image_path)
    # Wycięcie fragmentu z drop checkerem (parametry do ustalenia)
    drop_checker = img[100:200, 150:250] 
    
    hsv = cv2.cvtColor(drop_checker, cv2.COLOR_BGR2HSV)
    avg_hue = np.mean(hsv[:,:,0]) # Średni odcień
    
    if avg_hue > 160: return "LOW"
    elif avg_hue > 80: return "OPTIMAL"
    else: return "DANGER_HIGH"
```

#### Zastosowanie:
*   **Automatyczny wyłącznik:** Jeśli system wykryje kolor żółty, Home Assistant przez MQTT natychmiast wyłącza gniazdko z elektrozaworem CO2.
*   **Logi:** Możesz sprawdzić, jak szybko po włączeniu światła poziom CO2 staje się optymalny.

---

## 🚀 Funkcjonalności AI

System Smart-Aqua-Lab wykracza poza zwykłe zbieranie danych:
*   **Rozpoznawanie Gatunków:** Model CNN klasyfikujący mieszkańców akwarium.
*   **Analiza Behawioralna:** Detekcja apatii lub nadpobudliwości ryb, co może sygnalizować chorobę lub gwałtowną zmianę parametrów wody.
*   **System Wczesnego Ostrzegania:** Predykcja dryfu parametrów pH na podstawie danych historycznych (Pandas).

---

## 📦 Instalacja

1.  **Klonowanie repozytorium:**
    ```bash
    git clone https://github.com/twoje-repo/smart-aqua-lab.git
    ```
2.  **Deployment warstwy serwerowej:**
    ```bash
    docker-compose up -d
    ```
3.  **Flashowanie urządzeń:**
    *   Użyj narzędzia `esphome run` dla plików `nodes/sensors.yaml` oraz `nodes/camera.yaml`.

---

## 📈 Planowany Rozwój

*   [ ] Implementacja automatycznego dozownika pokarmu (PWM/Servo).
*   [ ] Automatyczna korekta pH za pomocą elektrozaworu CO2.
*   [ ] Rozbudowa dashboardu w Grafanie o zaawansowaną analitykę kosztów energii.

---

## ⚠️ Uwagi Dotyczące Bezpieczeństwa
*   Pamiętaj o odizolowaniu części elektronicznej od bezpośredniego kontaktu z wodą.
*   Zaleca się stosowanie gniazd z bezpiecznikiem różnicowoprądowym (RCD) przy urządzeniach zanurzeniowych.

---
*Projekt rozwijany w celach edukacyjno-hobbystycznych. Licencja: [Do uzupełnienia]*