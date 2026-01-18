# Lista wyszukamych dokumentow
- https://forum.arturhome.pl/t/home-assistant-miernik-ph-do-akwarium-esphome/2603 
- https://github.com/mlauhalu/esphome-phsensor 
- https://raaflahar.medium.com/ph-4502c-sensor-diymore-how-to-use-and-calibrate-using-arduino-uno-r3-3afc2b96631
- https://www.usinainfo.com.br/outros-sensores/sensor-de-ph-para-arduino-modulo-de-leitura-bnc-ph4502c-5316.html?srsltid=AfmBOorrtoDLSn-G8MNoRZ8mWkiPaRWe5Y9W3Lih67kKvK8X0-IkuFeu


## wyswietlacz
- https://esphome.io/components/display/ssd1306/ 
### wytyczne do kalibracji 
Ważne uwagi dotyczące PH-4502C:
1. Zasilanie: Moduł pH-4502C musi być zasilany stabilnym napięciem 5V. ESP8266 (NodeMCU) ma pin VIN lub 5V, który możesz wykorzystać, jeśli zasilasz płytkę przez USB.
2. Pin A0 na NodeMCU:
 - NodeMCU ma wbudowany dzielnik napięcia (rezystory 220k/100k).
 - Gdy na pin A0 podasz 3.3V, ESPHome odczyta to jako 1.0. Dlatego w kodzie dodałem filtr multiply: 3.3, abyś na wyświetlaczu widział faktyczne napięcie.
3. Kalibracja (Kluczowy krok!):
- Moduł posiada dwa potencjometry. Jeden służy do ustawiania progu wyjścia cyfrowego (nas nie interesuje), a drugi (bliżej złącza sondy) do Offsetu.
- Zanurz sondę w roztworze pH 7.0.
- Odczytaj napięcie na wyświetlaczu (Strona 2: NAPIECIE A0).
- Kręć potencjometrem, aż napięcie wyniesie około 2.5V (lub wpisz odczytaną wartość do zmiennej v_neutral w kodzie).
- Następnie sprawdź roztwór pH 4.0 i dostosuj mnożnik (slope) w kodzie, jeśli wynik pH się nie zgadza.
4. Wspólna masa (GND): Pamiętaj, aby masa (GND) zasilacza, modułu pH i NodeMCU była połączona.

Jak działa przełączanie ekranów?

Dodałem sekcję interval, która co 5 sekund wywołuje komendę display.page.show_next. Dzięki temu nie musisz nic klikać – wyświetlacz sam będzie "kręcił" wynikami: Temperatura -> Napięcie -> pH.
