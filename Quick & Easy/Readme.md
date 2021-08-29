## Schnell & Einfach / Quick and Easy

Wer wenig Programmiererfahrung hat oder mal eben schnell das Projekt realisieren möchte, kann die Schaltung sehr einfach nachbauen.
Dazu einfach die benötigten elektronischen Bauteile besorgen und die Schaltung auf einem Steckbrett oder einer Lochrasterplatine aufbauen.
Den ESP8266 per USB-Kabel an einen PC anschließen und die akzuelle .bin Datei hochladen.
Danach die persönlichen Einstellungen vornehmen.
FERTIG! :smiley:
<br>
<br>
If you have little programming experience or just want to realize the project quickly, you can build the circuit very easily.
Just get the needed electronic components and build the circuit on a breadboard.
Connect the ESP8266 via USB cable to a PC and upload the current .bin file.
Then make your personal settings.
DONE! :smiley:
<br>
<br>

## Stückliste / Bill of material
* Display:   TJCTM24028-SPI 2,8" TFT LCD Display Modul ILI9341 240x320
* Microcontroller: ESP8266 Wemos D1 mini
* 1x C= 470µF / 6,3V
* 2x R= 10k
* 2x Pushbutton
<br>
<br>

## Stromlaufplan / Schematic
![WebMonitor_1_Easy_Schematic](https://user-images.githubusercontent.com/88629497/131264949-6af7fe83-c49a-4b0c-b939-f8cb42a9b8bb.jpg)
<br>
<br>

## Software und Einstellungen

Eine geeignete Download Software für den ESP8266 von GitHub herunterladen.
Zum Beispiel ESPEasyFlasher  (FlashESP8266.exe):
* https://github.com/Nerdiyde/ESPEasyFlasher  (favorisiert)
* https://github.com/BattloXX/ESPEasyFlasher

<br>
Mittels der Download Software die Binärdatei (.bin Datei in diesem Verzeichnis) mit dem Programmcode auf den ESP8266 laden.
<br>
<br>

![Screenshot ESPEasyFlasher](https://user-images.githubusercontent.com/88629497/131265410-0bae8d90-c5a3-41a4-96e8-776a2447a0a9.png)
<br>

Vorbereitung des Routers:
Für die Nutzung von TR-064 muss diese Funktion im Router freigeschaltet sein.
Dies kann bei einer Fritz!Box je nach Modell und Firmware an einer anderen Stelle der Menüs zu finden sein.
In der Firmware 07.12 muss beispielsweise unter Heimnetz -> Netzwerk -> Netzwerkeinstellungen -> Heimnetzfreigaben ein Haken gesetzt werden.
Aktuelle Informationen dazu kann man im Internet zum Beispiel auf www.AVM.de finden.
Außerdem ist es sinnvoll einen eigenen User mit einem eigenen Passwort einzurichten, den man für den Zugang des WebMonitors benutzt.
<br>
<br>

![FB_TR064_Access](https://user-images.githubusercontent.com/88629497/131265540-2a46f1cf-0076-48f6-8a30-b59ca05e21f7.jpg)
![FB_User](https://user-images.githubusercontent.com/88629497/131265570-e8245f0e-2dc2-4c7d-94bb-09c1f41791cb.jpg)

<br>
Beim ersten Start der Software sieht man einen etwas chaotischen Bildschirm, weil noch keine gültigen Daten im EEPROM stehen.
Deshalb muss man den Taster S1 (am ESP8266 Pin D4) mit dem Computer ins Netzwerk >5s lang drücken und danach den RESET Knopf am ESP8266 D1 Mini.
Die Software startet neu und der ESP8266 baut ein neues offenes WLAN ohne Passwort mit dem Namen "Web-Monitor" auf.
Mit dem Computer in das Netzwerk "Web-Monitor" verbinden und mit einem Browser die Seite http://192.168.4.1 aufrufen.
<br>
<br>

![Setup_Website](https://user-images.githubusercontent.com/88629497/131265374-99f9ed93-a575-4f9d-9543-3d3ee1b6a5f9.jpg)
<br>

Die Felder mit den Zugangsdaten ausfüllen und den gelben Button "Alle Werte übernehmen" anklicken.
Danach können die getätigten Einstellungen am Display des WebMonitors überprüft werden.
Abschließend irgendeinen Taster des WebMonitors kurz drücken damit die Software durchstartet und korrekt arbeitet.
Fehlerhafte Einstellungen können korrigiert werden indem ein Taster des WebMonitors länger als 8s gedrückt wird.
Dadurch werden die im EEPROM gespeicherten Einstellungen gelöscht und die Software startet neu.
Nun können, wie bereits oben beschrieben, alle Einstellungen neu vorgenommen werden.
<br>
<br>

## Software and setup

Download a suitable download software for the ESP8266 from GitHub.
For example ESPEasyFlasher (FlashESP8266.exe):
* https://github.com/Nerdiyde/ESPEasyFlasher (favorited)
* https://github.com/BattloXX/ESPEasyFlasher

<br>
Using the download software, load the binary file (.bin file in this directory) with the program code to the ESP8266.
<br>
<br>

![Screenshot ESPEasyFlasher](https://user-images.githubusercontent.com/88629497/131265410-0bae8d90-c5a3-41a4-96e8-776a2447a0a9.png)
<br>

Preparation of the router:
To use TR-064, this function must be enabled in the router.
This can be found in a Fritz!Box depending on the model and firmware at a different point of the menus.
In firmware 07.12, for example, a check mark must be set under Home network -> Network -> Network settings -> Home network shares.
Current information on this can be found on the Internet, for example at www.AVM.de.
In addition, it is useful to set up a separate user with its own password, which you use to access the WebMonitor.
<br>
<br>

![FB_TR064_Access](https://user-images.githubusercontent.com/88629497/131265540-2a46f1cf-0076-48f6-8a30-b59ca05e21f7.jpg)
![FB_User](https://user-images.githubusercontent.com/88629497/131265570-e8245f0e-2dc2-4c7d-94bb-09c1f41791cb.jpg)

<br>
At the first start of the software you see a somewhat chaotic screen, because there is no valid data in the EEPROM yet.
Therefore you have to press the button S1 (at the ESP8266 pin D4) with the computer into the network for >5s and then press the RESET button at the ESP8266 D1 Mini.
The software restarts and the ESP8266 establishes a new open WLAN without password named "Web-Monitor".
Connect the computer to the network "Web-Monitor" and open the page http://192.168.4.1 with a browser.
<br>
<br>

![Setup_Website](https://user-images.githubusercontent.com/88629497/131265374-99f9ed93-a575-4f9d-9543-3d3ee1b6a5f9.jpg)
<br>

Fill in the fields with the access data and click the yellow button "Apply all values".
After that, the settings made can be checked on the display of the WebMonitor.
Finally, press any button of the WebMonitor briefly so that the software starts and works correctly.
Incorrect settings can be corrected by pressing a button on the WebMonitor for longer than 8 seconds.
This will erase the settings stored in the EEPROM and the software will restart.
Now, as already described above, all settings can be made again.
<br>
<br>

