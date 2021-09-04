# Schaltung und Layout
Die Schaltung und das Layout sind mit dem CAD-Programm Target3001! gezeichnet worden.
Das Programm kann in einer kostenlosen Version fürs Hobby bei https://ibfriedrich.com heruntergeladen werden.

## Der Stromlaufplan / The schematic
Der Stromlaufplan ist sehr einfach gehalten. Im Grunde genommen ist nur eine Display Baugruppe "TJCTM24028-SPI" an das Board "Wemos ESP8266 D1 mini" angeschlossen.
<br>
Die Touch-Funktion vom Display ist zwar angeschlossen, wird aber für dieses Projekt nicht genutzt.
Der SD-Card Anschluss des Display Boards ist ebenfalls angeschlossen.
Er war ursprünglich zum Speichern der Konfigurationsdaten vorgesehen, wird aber in diesem Projekt ebenfalls nicht genutzt.
Die beiden LEDs inklusive ihrer Vorwiderstände wurden nur zum Debugging genutzt und haben keine richtige Funktion mehr. 
Die Schaltung besitzt vier USB Anschlüsse mit unterschiedlichen Steckerverbindern und Ausrichtungen.
<br>
Sie dienen zur Spannungsversorgung des Boards. Es wird aber in der Praxis nur eine Buchse benötigt. Der Rest ist optional verfügbar.
Sie dienen nur dazu für den jeweiligen Einbauort die beste Variante auswählen zu können.
Man kann natürlich auch auf alle USB-Buchsen verzichten und die Schaltung über die Mini-USB Buchse des "Wemos ESP8266 D1 mini" versorgen.

![grafik](https://user-images.githubusercontent.com/88629497/132067967-2f39bc58-325b-4394-a611-8a7c63af4411.png)


## Das Board / The board

Aufbau
Stückliste

![grafik](https://user-images.githubusercontent.com/88629497/132068330-f04bb45b-dc5b-46f8-8620-b2b8d2f60e1a.png)



