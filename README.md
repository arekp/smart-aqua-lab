# Smart-Aqua-Lab

System inteligentnego monitorowania i zarządzania akwarium

## 📝 Opis projektu

Smart-Aqua-Lab to kompleksowe rozwiązanie do monitorowania parametrów akwarium, wykorzystujące nowoczesne technologie IoT i sztuczną inteligencję. Projekt łączy w sobie funkcje pomiarowe, monitoring wizyjny oraz inteligentną analizę stanu zdrowia ryb.

## 🎯 Główne funkcjonalności

### Monitoring parametrów wody
- Ciągły pomiar pH wody
- Monitoring temperatury w czasie rzeczywistym
- Automatyczne powiadomienia o odstępstwach od normy

### System wizyjny
- Podgląd akwarium online 24/7
- Automatyczne rozpoznawanie gatunków ryb
- Analiza zachowania i stanu zdrowia ryb
- Detekcja anomalii w zachowaniu mieszkańców akwarium

### Integracja i analiza danych
- Pełna integracja z Home Assistant
- Gromadzenie i wizualizacja danych historycznych
- System wczesnego ostrzegania
- Automatyczne powiadomienia o nieprawidłowościach

## 💻 Wykorzystane technologie

### Backend i analiza danych
- **Python FastAPI** - serwer REST API
- **InfluxDB** - baza danych szeregów czasowych do przechowywania odczytów z czujników
- **Pandas** - analiza danych historycznych
- **TensorFlow/OpenCV** - modele AI do rozpoznawania ryb i analizy ich zachowania
- **MQTT** - komunikacja między urządzeniami IoT

### Frontend
- **React** - interfejs użytkownika
- **Grafana** - wizualizacja danych z czujników
- **Socket.IO** - transmisja wideo w czasie rzeczywistym
- **Material-UI** - komponenty interfejsu użytkownika

### IoT i Embedded
- **PlatformIO** - środowisko programistyczne dla ESP32/ESP8266
- **Arduino Framework** - framework dla mikrokontrolerów
- **ESPHome** - integracja z Home Assistant
- **FreeRTOS** - system operacyjny czasu rzeczywistego dla ESP32

### DevOps i monitoring
- **Docker** - konteneryzacja aplikacji
- **Docker Compose** - orkiestracja kontenerów
- **Prometheus** - monitoring systemu
- **Mosquitto** - broker MQTT
- **Nginx** - serwer proxy

## 🛠️ Komponenty sprzętowe

### Sensory i kontrolery
- Moduł pomiaru pH (PH4502C)
- Czujniki temperatury
- Arduino (kontroler główny)
- ESP8266 (komunikacja Wi-Fi)
- ESP32-CAM (monitoring wizyjny)

### Wymagania dodatkowe
- Stabilne połączenie internetowe
- Serwer Home Assistant
- Zasilanie awaryjne (opcjonalnie)

## 📊 Architektura systemu

System składa się z trzech głównych warstw:
1. Warstwa sprzętowa (sensory i kontrolery)
2. Warstwa komunikacji (ESP8266/Wi-Fi)
3. Warstwa analityczna (serwer z Home Assistant)

## 🔧 Instalacja i konfiguracja

*Szczegółowa dokumentacja w przygotowaniu*

## 📚 Biblioteki i zależności

- [Biblioteka PH4502C](https://github.com/nthnn/PH4502C-Sensor?tab=readme-ov-file) - obsługa czujnika pH
- *Lista będzie uzupełniana*

## 📈 Planowany rozwój

- Dodanie automatycznej regulacji parametrów wody
- Rozszerzenie modeli AI o nowe gatunki ryb
- Implementacja zaawansowanej analizy predykcyjnej
- Integracja z systemami automatycznego karmienia

## 📄 Licencja

*Do uzupełnienia*

## 👥 Współpraca

Jesteśmy otwarci na współpracę i sugestie. Jeśli chcesz przyczynić się do rozwoju projektu:
1. Zrób fork repozytorium
2. Stwórz branch z Twoimi zmianami
3. Wyślij Pull Request

## ⚠️ Status projektu

Projekt jest w fazie aktywnego rozwoju. Niektóre funkcjonalności mogą być niestabilne lub w trakcie implementacji.