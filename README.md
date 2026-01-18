Oto poprawiona i rozszerzona wersja dokumentu `README.md`. Została ona ustrukturyzowana jako profesjonalna dokumentacja techniczna, integrująca informacje o sprzęcie, konfiguracji programowej (ESPHome) oraz procedurach kalibracji.

---

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
| **Czujnik Jakości** | TDS Meter v1.0 | Pomiar czystości wody |
| **Zasilanie** | Stabilizowane 5V DC | Zasilanie modułów pH i ESP |

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