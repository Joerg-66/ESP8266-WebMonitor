# Software
## Die Entwicklungsumgebung

Lade und installiere von der Seite https://www.arduino.cc/die Software Arduino (Version 1.8.15)

Istalliere die folgenden Arduino Bibliotheken:
* Adafruit GFX Library	(Version 1.10.10)
* Adafruit BusIO			(Version 1.9.0)
* Adafruit ILI9341		(Version 1.5.9)
* Adafruit STMPE610		(Version 1.1.3)
* Adafruit TouchScreen	(Version 1.1.2)

Lade und installiere die TR-064 Bibliotheken von:
* https://github.com/Aypac/Arduino-TR-064-SOAP-Library		(Version: Feb 2020)
Diese Bibliothek ist eine großartige Arbeit von René Vollmer und übernimmt die TR-064 Kommunikation zum Router.
Um diese Bibliothek nicht zu modifizieren, wurden die TR064_Query.h und TR064_Query.cpp als Zwischenschicht geschrieben.

In der Arduino IDE unter "Voreinstellungen" im Feld "Zusätzliche Boardverwalter-URLs den folgenden Eintrag hinzufügen:
* https://arduino.esp8266.com/stable/package_esp8266com_index.json

Nun kann in der Arduino IDE über den Boardverwalter das "ESP Community" Packet installiert weden:
ESP8266 Community		(Version 2.7.4)
ACHTUNG! Aktuell muss das etwas ältere Paket wählen. Die Pakete Version 3.x.x verursachen einen Konflikt mit der TR-064-SOAP-Library.

Nun kann in der Arduino IDE das Board "LOLIN(WEMOS) D1 R2 & mini" ausgewählt werden.
Alle Werte behalten die Default Werte. Die Flash Size ist "4MB (FS:2MB OTA:~1019KB)"



