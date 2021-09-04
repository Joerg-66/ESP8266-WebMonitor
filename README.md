# WebMonitor
## Motivation
Im ersten Covid-19 Lockdown 2020 in Deutschland hatte ich einen sehr langsamen Internetzugang und war zeitweilig im Home-Office.
Bei Problemen in Videokonferenzen wollte ich der Ursache auf den Grund gehen und wissen wie die aktuelle Netzwerklast zum Internet war.
Zwar hatte mein derzeitiger Router, eine Fritz!Box 7490, eine Übersicht darüber auf einer internen Website aber ich wollte die Informationen unabhängig vom Browser angezeigt bekommen.
Die Idee war es einen ESP8266 Controller per WLAN an den Router zu koppeln und die benötigten Informationen per TR-064 Protokoll auszulesen.
Daraus ist dieses kleine Hard- und Softwareprojekt hervorgegangen.
<br>
***Es wurde mit den Routermodellen Fritz!Box 7490, Fritz!Box 7530 und Fritz!Box 7590 der Firma AVM getestet, sollte aber auch mit anderen Modellen des Herstellers funktionieren.***
<br>
<br>

In the first Covid-19 lockdown 2020 in Germany, I had a very slow Internet access and was temporarily in the home office.
With problems in video conferences, I wanted to know the root of the cause and know what the current network load to the Internet was.
Although my current router, a Fritz!Box 7490, had an overview of this on an internal website, I wanted to have the information displayed independently of the browser.
The idea was to connect an ESP8266 controller via wifi to the router and read out the needed information via TR-064 protocol.
This tiny hardware and software project was the result.
<br>
***It was tested with the router models Fritz!Box 7490, Fritz!Box 7530 and Fritz!Box 7590 from the company AVM, but should also work with other models of the manufacturer.***
<br>
<br>
<br>
<br>
![Board_02](https://user-images.githubusercontent.com/88629497/131263851-c4c651b0-937a-43f5-a703-0f1527c2069a.jpg)
<br>
<br>

## Ziele

* Überprüfung ob die Internetverbindung steht
* WLAN Verfügbarkeit
* Erkennung ob ein Gastzugang aktiv ist
* max. Upload Bandbreite
* aktuelle Upload Rate
* Upload Überblick der letzten Minuten
* max. Download Bandbreite
* aktuelle Download Rate
* Download Überblick der letzten Minuten

## Goals
* Check if the internet connection is up
* wifi availability
* detection if guest access is active
* max. upload bandwidth
* current upload rate
* Upload overview of the last minutes
* max. download bandwidth
* current download rate
* download overview of the last minutes
<br>

![LCD_03](https://user-images.githubusercontent.com/88629497/131263840-388d4b19-04d7-47f2-a3be-433c8e06c6d0.jpg)

