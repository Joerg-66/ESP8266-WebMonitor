/*
 * Kommunikation zu TR-064 kompatiblem Router
 * Autor: Jörg Bernhardt
 * 
 * Board: LoLin ESP8266 D1 Mini
 * 
 * Arduino Settings:
 *    Board:  "LOLIN(WEMOS) D1 R2 & Mini"
 *    Upload Speed: "921600"
 *    CPU Frequency: "80MHz"
 *    Flash Size: "4MB (FS:2MB OTA:~1019KB)"
 *    Debug Port: "Disabled"
 *    Debug Level: keine"
 *    
 * 
 * 
 * https://github.com/adafruit/Adafruit_ILI9341
 * https://github.com/adafruit/Adafruit-GFX-Library
 * https://github.com/dancol90/ESP8266Ping
 * https://github.com/marian-craciunescu/ESP32Ping
 * https://github.com/Aypac/Arduino-TR-064-SOAP-Library
 * 
 */

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WiFiMulti.h>
  #include <ESP8266HTTPClient.h>
  ESP8266WiFiMulti WiFiMulti;
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WiFiMulti.h>
  #include <HTTPClient.h>
  WiFiMulti WiFiMulti;
#endif

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#include "ILI9341_Graphic.h"
#include "ESP_Settings.h"
#include "TR064_Query.h"


#define TFT_CS      15    // TFT CS  Pin ist verbunden mit NodeMCU Pin D8
#define TFT_RST     16    // TFT RST Pin ist verbunden mit NodeMCU Pin D0
#define TFT_DC      5     // TFT DC  Pin ist verbunden mit NodeMCU Pin D1
#define LED_RED     1     // Rote LED ist verbunden mit NodeMCU Pin TX
#define LED_GREEN   3     // Grüne LED ist verbunden mit NodeMCU Pin RX
#define SWITCH_01   2     // Taster S1 ist verbunden mit NodeMCU Pin D4
#define SWITCH_02   A0    // Taster S2 ist verbunden mit NodeMCU Pin A0

#define ILI9341_DARKORANGE 0xCB05
#define FirmwareVersion "1.3"

ILI9341_Graphic Display = ILI9341_Graphic(TFT_CS, TFT_DC, TFT_RST);       // "Display" = neue Instanz der Klasse "ILI9341_Graphic"
ESP_Settings MyESP;                                                       // "MyESP" = neue Instanz der Klasse "ESP_Settings"
TR064_Query TR_064 = TR064_Query(49000, "192.168.178.254", "admin", "");  // "TR_064" = neue Instanz der Klasse "TR064_Query"
Ticker TickTimer;

const char DEVICE_NAME[] = "ESP8266_TR064";
int iPosX = 10;
int iPosY = 20;
int iCurrentMode = 0;
int iTickerCount = 0;
int iOldTickerCount = 0;
int iDummy = 0;
long lDummy = 0;
long lUpNow = 0;
long lUpMax = 0;
long lDownNow = 0;
long lDownMax = 0;
long lStartTime = 0;
String sConnType = "";
String sRouterIP = "";
IPAddress ipRouter;
String sESP8266IP = "";
IPAddress ipESP8266;
bool bNetworkAvailable;
bool bWifiAvailable;
bool bGuestActive;
bool bWebAvailable;
int iWhileCounter;
long lWifiSignal;
String sConnection;

unsigned long currentMillis_1;
unsigned long currentMillis_2;
        
enum PrgState {
  NORMAL,
  CONFIG,
  DEMO,
  GRAPHIC,
  TEXT
};

enum PrgState ProgState = NORMAL;

void TickerCounter(void);


void setup(){
  ESP.wdtDisable();             // Disable ESP8266 Software Watchdog
    
  String sWifiSSID = MyESP.Read_WifiSSID();
  String sWifiPassword = MyESP.Read_WifiPassword();
  String sTR064_User = MyESP.Read_TR064_User();
  String sTR064_Password = MyESP.Read_TR064_Password();
  unsigned long ulMaxDL_Speed = MyESP.Read_Download_Speed();
  String sMaxDL_Speed = String(ulMaxDL_Speed);
  unsigned long ulMaxUL_Speed = MyESP.Read_Upload_Speed();
  String sMaxUL_Speed = String(ulMaxUL_Speed);
  
  Serial.begin(115200);

  pinMode(SWITCH_01, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_RED, LOW); 
  digitalWrite(LED_GREEN, LOW); 
  
  Display.begin();
  Display.Setup();
  
  ProgState = NORMAL;
  if((analogRead(SWITCH_02) < 512) || (digitalRead(SWITCH_01) == LOW)){
    ProgState = DEMO;
    ArduinoOTA.begin();
  }

  if(ProgState == NORMAL){
    Display.Logo(190,10);
    Display.setTextSize(1);
    Display.setCursor(207, 100);
    Display.setTextColor(ILI9341_CYAN);
    Display.print("FW-Version: "); Display.print(FirmwareVersion);
    Display.setTextColor(ILI9341_WHITE);
    Display.setCursor(10, 25);
    Display.println("Konfiguration starten.");
    iPosY = iPosY + 25; Display.setCursor(iPosX, iPosY);
    sWifiSSID.trim();
    if(sWifiSSID == ""){                                // Sind Netzwerkeinstellungen im EEPROM gespeichert?    
      WiFi.disconnect();
      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
      WiFi.softAP("Web_Monitor");                  // SSID = "Web_Monitor" [kein Passwort]
      delay(500);
      Display.MessageScreen_01("192.168.4.1");
      bool bShowMsgScr = true;
      while(1){
        MyESP.WebInterface(true);   // Website mit Konfigurationsmöglichkeiten anzeigen
        delay(5);
        sWifiSSID = MyESP.Read_WifiSSID(); sWifiSSID.trim();
        if(sWifiSSID.length() > 3){
          sWifiPassword = MyESP.Read_WifiPassword(); sWifiPassword.trim();
          sTR064_User = MyESP.Read_TR064_User(); sTR064_User.trim();
          sTR064_Password = MyESP.Read_TR064_Password(); sTR064_Password.trim();
          ulMaxDL_Speed = MyESP.Read_Download_Speed(); sMaxDL_Speed = String(ulMaxDL_Speed / 1000);
          ulMaxUL_Speed = MyESP.Read_Upload_Speed(); sMaxUL_Speed = String(ulMaxUL_Speed / 1000);
          if(bShowMsgScr == true) Display.MessageScreen_02(sWifiSSID,sWifiPassword, sTR064_User, sTR064_Password, sMaxDL_Speed, sMaxUL_Speed);
          bShowMsgScr = false;
        }
        if((analogRead(SWITCH_02) < 512) || (digitalRead(SWITCH_01) == LOW)) ESP.restart();
      }
    }
    else{
      sWifiSSID = MyESP.Read_WifiSSID(); sWifiSSID.trim();
      sWifiPassword = MyESP.Read_WifiPassword(); sWifiPassword.trim();
      Display.println(sWifiSSID + " verbinden:");
      iPosY = iPosY + 15; Display.setCursor(iPosX, iPosY);
      WiFi.hostname("Web_Monitor");
      WiFi.mode(WIFI_STA);
      iDummy = 0;
      WiFi.begin(sWifiSSID, sWifiPassword);
      while ((WiFi.status() != WL_CONNECTED)){
        Display.print(".");
        ++iDummy;
        delay(400 + 100 * iDummy);

        if(digitalRead(SWITCH_01) == LOW){
          Display.MessageScreen_03();
          lStartTime = millis();
          while(1){
            if ((millis() - lStartTime) > 8000){
              MyESP.DeleteEEPROM();
              Display.MessageScreen_04();
              delay(5000);
              ESP.restart(); 
              delay(500);
            }
            if(digitalRead(SWITCH_01) == HIGH) ESP.restart();
            delay(500);
          }
        }
        
        if(iDummy > 25) break;
        ESP.wdtFeed();
      }


      if(iDummy <= 25){
        iPosY = iPosY + 25; Display.setCursor(iPosX, iPosY);
        Display.println("WLAN-Verbindung hergestellt.");
        delay(500);
        ipRouter = WiFi.dnsIP();
        sRouterIP = String(ipRouter[0]) + "." + String(ipRouter[1]) + "." + String(ipRouter[2]) + "." + String(ipRouter[3]);
        iPosY = iPosY + 20; Display.setCursor(iPosX, iPosY);
        Display.println("Router IP: " + sRouterIP);
        ipESP8266 = WiFi.localIP();
        sESP8266IP = String(ipESP8266[0]) + "." + String(ipESP8266[1]) + "." + String(ipESP8266[2]) + "." + String(ipESP8266[3]);
        iPosY = iPosY + 20; Display.setCursor(iPosX, iPosY);
        Display.println("Eigene IP: " + sESP8266IP);
        iPosY = iPosY + 20; Display.setCursor(iPosX, iPosY);
        sTR064_User = MyESP.Read_TR064_User(); sTR064_User.trim();
        TR_064.~TR064_Query();                                                      // Default-Instanz "TR_064" mittels Destructor löschen
        new(&TR_064) TR064_Query(49000, sRouterIP, sTR064_User, sTR064_Password);   // Neue Instanz "TR_064" mit aktuellen Parametern erzeugen
        TR_064.init();
        iDummy = TR_064.getDeviceNumber();
        if(iDummy != 0){
          Display.println("TR-064 Verbindung hergestellt.");
          ProgState = NORMAL;  
        }
        else{
          Display.println("TR-064 Verbindung gescheitert.");
          ProgState = CONFIG;  
        }
      }
      else ProgState = CONFIG;
        
      if(ProgState == CONFIG){
        Display.setTextColor(ILI9341_YELLOW);
        iPosY = 175; Display.setCursor(iPosX, iPosY);
        Display.println("Taster S1 kurz dr\201cken, um neu zu starten.");
        iPosY = iPosY + 15; Display.setCursor(iPosX, iPosY);
        Display.println("Taster S1 >5s dr\201cken, um die existierende");
        iPosY = iPosY + 15; Display.setCursor(iPosX, iPosY);
        Display.println("Konfiguration zu l\224schen und neu einzugeben.");
        iPosY = iPosY + 15; Display.setCursor(iPosX, iPosY);
        Display.println("Taster S2 kurz dr\201cken, um Demo-Modus zu starten.");   
        while(1){
          ESP.wdtFeed();
          delay(10);
          long lTimeS1 = S1_msTime();
          if(lTimeS1 > 50){
            if(lTimeS1 > 5000) MyESP.DeleteEEPROM();
            ESP.restart();        
          }
          long lTimeS2 = S2_msTime();
          if(lTimeS2 > 50){
            ProgState = DEMO;
            break;
          }
        }
      }
    }

    sConnType = TR_064.getDefaultConnectionService();
    if(sConnType.indexOf("WANPPP") != -1) sConnType = "WAN-PPP";
    else sConnType = "WAN-IP";
    iPosY = iPosY + 25; Display.setCursor(iPosX, iPosY);
    Display.println("Es existiert eine " + sConnType + " Internetverbindung.");
    Display.setTextColor(ILI9341_YELLOW);
    iPosY = iPosY + 25; Display.setCursor(iPosX, iPosY);
    Display.print("Programm starten ");
    for (int i=0; i<20; i++){
      Display.print(".");
      delay(500);
      ESP.wdtFeed();
    }
    
    if(ulMaxUL_Speed<1000){
      lUpMax = long(TR_064.getUpstreamMaxBitRate());
    }
    else lUpMax = ulMaxUL_Speed;
    if(ulMaxDL_Speed<1000){
      lDownMax = long(TR_064.getDownstreamMaxBitRate());
    }
    else lDownMax = ulMaxDL_Speed;
  
    DrawStartScreen(lUpMax, lDownMax);
    bNetworkAvailable = true;
    bWebAvailable = true;
    bWifiAvailable = true;
    
    TickTimer.attach(0.8, TickerCounter);
  }
}

//###########################################################################################################

void loop(){

  ESP.wdtFeed();
  if(ProgState == DEMO) ShowDemo();
    
  if(iTickerCount != iOldTickerCount){
    iOldTickerCount = iTickerCount;
    MyESP.WebInterface(false);                                          // Website mit Konfigurationsmöglichkeiten anzeigen
    if(bWebAvailable == true){
      Display.Connection(155, 38, true);
    }
    else{
      Display.Connection(155, 38, false);
    }

    if((analogRead(SWITCH_02) < 512) || (digitalRead(SWITCH_01) == LOW)){
      Display.MessageScreen_03();
      lStartTime = millis();
      while(1){
        ESP.wdtFeed();
        if((analogRead(SWITCH_02) > 512) && (digitalRead(SWITCH_01) == HIGH)) break;
        if ((millis() - lStartTime) > 8000){
          MyESP.DeleteEEPROM();
          Display.MessageScreen_04();
          delay(5000);
          ESP.restart(); 
        }
      }
      Display.fillScreen(ILI9341_BLACK);
      Display.Cloud(10, 85, ILI9341_CYAN);
      Display.Computer(10, 195, ILI9341_GREEN);
      Display.ArrowUpTiny(20, 107, ILI9341_CYAN);
      Display.ArrowDownTiny(20, 175, ILI9341_GREEN);
      Display.TextBits(40, 65, lUpMax, ILI9341_LIGHTGREY);
      Display.TextBits(40, 155, lDownMax, ILI9341_LIGHTGREY);
      iTickerCount = 1;
    }

    if(iTickerCount == 1){                                              // TickerCount 1: WLAN-Verbindung und Erreichbarkeit des Routers überprüfen und darstellen    
      iWhileCounter = 0;
      while(1){                                                         // so lange in der Schleife laufen bis der Router per TR-064 erreicbar ist
        if(iWhileCounter > 0) delay(200);
        if(iWhileCounter > 10000) iWhileCounter = 10000;
        ++iWhileCounter;
        bWifiAvailable = false;
        bNetworkAvailable = false;
        if(WiFi.status() == WL_CONNECTED){                              // prüfen ob das WLAN verbunden ist
          lWifiSignal =  WiFi.RSSI();
          bWifiAvailable = true;
          if(TR_064.getDeviceNumber() != 0) bNetworkAvailable = true;   // prüfen ob der Router per TR-064 erreicbar ist
        }
        if(iWhileCounter > 3){                                          // nach >3 Schleifenduchläufen Display Daktivierung darstellen
          if(bWifiAvailable == false){
            Display.Wifi(12, 12, ILI9341_DARKGREY);                     // Wifi Signal auf grau stellen wenn kein WLAN erreichbar
          }
          else{
            Display.Wifi(12, 12, ILI9341_WHITE, String(lWifiSignal));   // Wifi Signal anzeigen wenn WLAN erreichbar ist
          }
          Display.Guest(150, 7, ILI9341_BLACK, ILI9341_BLACK);          // Symbole auf Deaktiviert stellen ...
          Display.FlashTiny(75, 10, ILI9341_YELLOW);                    // ... Blitz Symbol zwischen Wifi-Signal und Router anzeigen ...
          Display.Router(100, 12, false);
          Display.Connection(155, 35, false);
          Display.Earth(250, 10, false);
          if((iWhileCounter % 5) == 0){                                 // ... Bitraten in Grid-Arrays mit Wert 0 zyklisch vorschieben
            Display.Grid_01(80, 65, 0, lUpMax, ILI9341_CYAN);
            Display.TextBits(40, 65, lUpMax, ILI9341_LIGHTGREY);
            Display.TextBits(40, 80, 0, ILI9341_CYAN);
            Display.Grid_02(80, 155, 0, lDownMax, ILI9341_GREEN);
            Display.TextBits(40, 155, lDownMax, ILI9341_LIGHTGREY);
            Display.TextBits(40, 170, 0, ILI9341_GREEN);
          }
          iTickerCount = 0;                                             // iTickerCount auf 0 setzen für einen definierten Start nach neuem Kontakt zum Router
        }
        
        if(bNetworkAvailable == true){                                  // der Router konnte per TR-064 erreicht werden
          Display.FlashTiny(75, 10, ILI9341_BLACK);
          Display.Wifi(12, 12, ILI9341_WHITE, String(lWifiSignal));
          Display.Router(100, 12, true);
          if(bWebAvailable == true){
            Display.FlashTiny(210, 10, ILI9341_BLACK);
            Display.Earth(250, 10, true);
          }
          else{
            Display.FlashTiny(210, 10, ILI9341_YELLOW);
            Display.Earth(250, 10, false);
          }
          if(bGuestActive == true) Display.Guest(150, 7, ILI9341_ORANGE, ILI9341_DARKORANGE);
          else Display.Guest(150, 7, ILI9341_BLACK, ILI9341_BLACK); 
          break;                                                        // Router per TR-064 erreichbar --> Schleife verlassen
        }
      }      
    }

    if(iTickerCount == 2){                                              // TickerCount 2: aktuelle Upload- und Download-Geschwindigkeit abrufen
      lUpNow = long(TR_064.getNewByteUploadRate());
      lDownNow = long(TR_064.getNewByteDownloadRate()); 
    }

    if(iTickerCount == 3){                                              // TickerCount 3: Aktuelle Upload- und Download-Geschwindigkeit anzeigen
      Display.Grid_01(80, 65, lUpNow, lUpMax, ILI9341_CYAN);
      Display.TextBits(40, 65, lUpMax, ILI9341_LIGHTGREY);
      Display.TextBits(40, 80, lUpNow, ILI9341_CYAN);
      Display.Grid_02(80, 155, lDownNow, lDownMax, ILI9341_GREEN);
      Display.TextBits(40, 155, lDownMax, ILI9341_LIGHTGREY);
      Display.TextBits(40, 170, lDownNow, ILI9341_GREEN);
    }

    if(iTickerCount == 4){
      if(sConnType == "WAN-PPP") sConnection = TR_064.getWANPPPConnectionStatus();
      else if(sConnType == "WAN-IP") sConnection = TR_064.getWANIPConnectionStatus();
      else sConnection = "none";
      sConnection.toUpperCase();
      if(sConnection == "CONNECTED") bWebAvailable = true; else bWebAvailable = false;      
    }

    if(iTickerCount == 5){
      if(TR_064.getGuestWlanEnabled() == 0) bGuestActive = false; else bGuestActive = true;
    }
    
  }  
}


//###########################################################################################################

void TickerCounter(void){
  iTickerCount++;
  if(iTickerCount >= 7){
    iTickerCount = 0;
  }
}

void DrawStartScreen(long lUpMaxValue, long lDownMaxValue){
  Display.fillScreen(ILI9341_BLACK);
  Display.Connection(155, 35, false);
  Display.FlashTiny(75, 10, ILI9341_DARKGREY);
  Display.FlashTiny(210, 10, ILI9341_DARKGREY);
  Display.Wifi(12, 12, ILI9341_DARKGREY, "");
  Display.Cloud(10, 85, ILI9341_CYAN);
  Display.Computer(10, 195, ILI9341_GREEN);
  Display.ArrowUpTiny(20, 107, ILI9341_CYAN);
  Display.ArrowDownTiny(20, 175, ILI9341_GREEN);
  Display.Router(100, 12, false);
  Display.Guest(150, 7, ILI9341_LIGHTGREY, ILI9341_DARKGREY);
  Display.Earth(250, 10, false);
  Display.TextBits(40, 65, lUpMaxValue, ILI9341_LIGHTGREY);
  Display.TextBits(40, 80, 0, ILI9341_CYAN);
  Display.Grid_01(80, 65, 0, lUpMax, ILI9341_CYAN);
  Display.TextBits(40, 155, lDownMaxValue, ILI9341_LIGHTGREY);
  Display.TextBits(40, 170, 0, ILI9341_GREEN);
  Display.Grid_02(80, 155, 0, lDownMax, ILI9341_GREEN);
}


long S1_msTime(void){
  static boolean bOldState = true;
  static unsigned long lStartMillis = 0;
  unsigned long lMillis;

  if(bOldState == true){
    if(digitalRead(SWITCH_01) == LOW){
      lStartMillis = millis();
      bOldState = false;
    }
  }
  else{
    if(digitalRead(SWITCH_01) == true){
      lMillis = millis();
      lMillis = lMillis - lStartMillis;
      bOldState = true;
      return long(lMillis);
    }
  }  
  return 0;
}
long S2_msTime(void){
  static boolean bOldState = true;
  static unsigned long lStartMillis = 0;
  unsigned long lMillis;

  if(bOldState == true){
    if(analogRead(SWITCH_02) < 512){
      lStartMillis = millis();
      bOldState = false;
    }
  }
  else{
    if(analogRead(SWITCH_02) > 512){
      lMillis = millis();
      lMillis = lMillis - lStartMillis;
      bOldState = true;
      return long(lMillis);
    }
  }  
  return 0;
}

void ShowDemo(void){
int iRndUpLoad;
int iRndDownLoad;
int iRndWifi;
int iLoopCounter;
int iCountNo2;
bool bError;

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_RED, HIGH);
  DrawStartScreen(2000, 16000);
  Display.setTextColor(ILI9341_WHITE, ILI9341_RED);
  Display.setCursor(96, 53);
  Display.print("  DEMO MODUS & OTA Programming  ");
  delay(1000);
  Display.Router(100, 12, true);
  Display.Guest(150, 7, ILI9341_BLACK, ILI9341_BLACK);
  Display.Earth(250, 10, true);
  Display.FlashTiny(75, 10, ILI9341_BLACK);
  Display.FlashTiny(210, 10, ILI9341_BLACK);
  iLoopCounter = 0;
  iCountNo2 = 0;
  bError = false;
  while(1){
    ESP.wdtFeed();
    delay(700);
    ArduinoOTA.handle();
    if(bError == false) Display.Connection(155, 38, true);
    if(iLoopCounter == 0){
      if(bError == false) iRndUpLoad = random(300, 1000); else iRndUpLoad = 0;
      Display.TextBits(40, 80, iRndUpLoad, ILI9341_CYAN);
      Display.Grid_01(80, 65, iRndUpLoad, 2000, ILI9341_CYAN);
      if(bError == false) iRndDownLoad = random(7000, 12000); else iRndDownLoad = 0;
      Display.TextBits(40, 170, iRndDownLoad, ILI9341_GREEN);
      Display.Grid_02(80, 155, iRndDownLoad, 16000, ILI9341_GREEN);
      iRndWifi = random(-64, -54);
      Display.Wifi(12, 12, ILI9341_WHITE, String(iRndWifi));
      iCountNo2++;
      if(iCountNo2 == 10) Display.Guest(150, 7, ILI9341_ORANGE, ILI9341_DARKORANGE);
      if(iCountNo2 == 20) Display.Guest(150, 7, ILI9341_BLACK, ILI9341_BLACK);
      if(iCountNo2 == 25){
        Display.FlashTiny(210, 10, ILI9341_YELLOW);
        Display.Connection(155, 38, false);
        Display.Earth(250, 10, false);
        bError = true;
      }
      if(iCountNo2 == 30){
        Display.Router(100, 12, false);
        Display.FlashTiny(75, 10, ILI9341_YELLOW);
        Display.FlashTiny(210, 10, ILI9341_BLACK);
        Display.Connection(155, 38, false);
        bError = true;
      }
      if(iCountNo2 == 37){
        Display.Router(100, 12, true);
        Display.FlashTiny(75, 10, ILI9341_BLACK);
        Display.FlashTiny(210, 10, ILI9341_BLACK);
        Display.Earth(250, 10, true);
        bError = false;
        iCountNo2 = 0;
      }
    }
    iLoopCounter++;
    if(iLoopCounter == 8) iLoopCounter = 0;
    if((analogRead(SWITCH_02) < 512) || (digitalRead(SWITCH_01) == LOW)) ESP.restart();
  }
}

