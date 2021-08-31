#include "ESP_Settings.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

static const char PROGMEM ExplanationText[] = "\
    <p><font size=\"2\"><br/><br/>F&uuml;r die Nutzung von TR-064 muss diese Funktion im Router freigeschaltet sein. \
    Dies kann bei einer Fritz!Box je nach Modell und Firmware an einer anderen Stelle der Men&uuml;s zu finden sein. \
    In der Firmware 07.12 muss beispielsweise unter <strong>Heimnetz</strong> -> <strong>Netzwerk</strong> -> <strong>Netzwerkeinstellungen</strong> -> <strong>Heimnetzfreigaben</strong> ein Haken gesetzt werden.\
    <br/>Aktuelle Informationen dazu kann man im Internet zum Beispiel auf <strong>www.AVM.de</strong> finden.<br/>\
    <br/><br/>Der <strong>WLAN Name</strong> ist der <strong>Name des WLAN-Funknetzes</strong> der Fritz!Box.<br/>\
    <br/>Das <strong>WLAN Passwort</strong> ist der <strong>WLAN-Netzwerkschl&uuml;ssel</strong> der Fritz!Box.<br/>\
    <br/>F&uuml;r <strong>TR-064 User</strong> und <strong>TR-064 Passwort</strong> gibt es 2 M&ouml;glichkeiten der Konfiguration:<br/>\
    <ol> <li>Der <strong>TR-064 User = \'admin\'</strong> und <strong>TR-064 Passwort =</strong> das <strong><em>Kennwort</em></strong> der Fritz!Box. (Das kann bei aktuellen Routern problematisch sein)</li><br/>\
    <li>In der Fritz!Box k&ouml;nnen eigene Benutzer angelegt werden. \
    <br/>In diesem Fall ist der <strong>TR-064 User =</strong> angelegter <em><strong>Benutzername</strong></em> und das <strong>TR-064 Passwort =</strong> zugheh&ouml;riges <em><strong>Benutzerkennwort</strong></em>.<br/>\
    </li></ol>Bei <strong>Download Speed</strong> und <strong>Upload Speed</strong> k&ouml;nnen die vom Provider zur Verf&uuml;gung gestellten Geschwindigkeiten in MBit/s manuell eingetragen werden.\
    <br/>Bei <strong>fehlendem Eingabewert</strong> oder dem <strong> Eingabewert = 0</strong> wird versucht die jeweilige vom Provider vorgegebene maximale Geschwindigkeit automatisch aus dem Router auszulesen.\
    <br/> Diese Geschwindigkeit dient zur Anpassung der grafischen Anzeige des Datenstromes.<br/><br/>\
    <br/>Nach Eingabe der Daten und Dr&uuml;cken des Knopfes <strong>\"Alle Werte &uuml;bernehmen\"</strong> hat der Mikrocontroller die neue Konfiguration. \
    Nun muss kurz die Versorgungsspannung ausgeschaltet werden. Beim n&auml;chsten Start sollte sich nun der Mikrocontroller automatisch im WLAN der Fritz!Box anmelden und eine dynamische IP-Adresse beziehen.\
    Wenn dies nicht funktioniert, dann muss der Mikrocontroller &uuml;ber die entsprechende Taste <em>(siehe Informationen auf dem Display)</em> zur&uuml;ckgesetzt und die Konfiguration erneut durchgef&uuml;hrt werden.\
    <br/><br/><br/><br/><br/></font></p>";

static const char PROGMEM WebMonitor_Picture[13000] = {"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"};

static const char PROGMEM Site_Picture[600] = {"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAACXBIWXMAAAsTAAALEwEAmpwYAAABRUlEQVQ4y9WTsUsCcRTHP0o3SGBjg7Q1tDj\
    dEv0FHoK0GELDb2uT23QzdLtokbY2B8NJ4hCultqki87FoeWmxLixBBH6pdfSDw5PW5x68Pj93uPLe7/3+fESoWTOBpZkQ9u4wNaq5MRh3C80KtHckV27SBtkYuJQMo/6p83bFG0xRVs86LxP0RZdircqXt\
    bHRth5DXfV/cMrPgOkyHYAhNe7+ZPBxGEMcM+x7eoyMBE+wIxhSWnutMbpWgb9QqOCRctE+HjCH1n58jZfEjAACZSpDkSsgOoM0K2eOCaCawYvZ1WumrT2U2Q7M4YlE+Gr10wcxmmDDKFkrqApgAqaOvfoX\
    S7DVPpktLtXX/2tIytfjsKMjeDqMgDQz/l26zJIednOwS/AJtiA4eoyyD3V2o+HBEqnA4l1u7BMOydr7VW6xP9fph9Jv7JH7MNGgQAAAABJRU5ErkJggg=="};
    
static boolean bFirstRun = true;
boolean bSaveToEEPROM = false;
boolean bValidInput = false;
boolean bValidInput01 = false;
boolean bValidInput02 = false;
boolean bValidInput03 = false;
boolean bValidInput04 = false;
boolean bValidInputLength = false;
boolean bValidInputLength01 = false;
boolean bValidInputLength02 = false;
boolean bValidInputLength03 = false;
boolean bValidInputLength04 = false;

String HTTP_header;
String sMessage = "";
char cWifi_Name[33] = "";
char cWifi_Password[65] = "";
char cTR064_User[65] = "";
char cTR064_Password[65] = "";
char cDL_Speed[8] = "";
unsigned long ulDL_Speed = 0;
byte bDL_Speed[4] = {0, 0, 0, 0};
char cUL_Speed[8] = "";
unsigned long ulUL_Speed = 0;
byte bUL_Speed[4] = {0, 0, 0, 0};

// Webserver auf Port 80 setzen
WiFiServer server(80);

void ESP_Settings::WebInterface(boolean bShowFullSite = true){
  String sDummy;
  int iDummy;
  boolean bSaveToEEPROM = false;

  if (bFirstRun == true){
    //server.serveStatic("/", SPIFFS, "/index.html"); // Die Abfrage auf die reine URL '/' wird auf '/index.html' umgelenkt
    server.begin();
    delay(100);
    bFirstRun = false;
  }

  
    yield();
    delay(10);
    WiFiClient client = server.available();   // Hört auf Anfragen von Clients
    
    if (client) {                             // Falls sich ein neuer Client verbindet,
      Serial.println("Neuer Client.");        // Ausgabe auf den seriellen Monitor
      String currentLine = "";                // erstelle einen String mit den eingehenden Daten vom Client
      
      while (client.connected()) {            // wiederholen so lange der Client verbunden ist
        if (client.available()) {             // Fall ein Byte zum lesen da ist,
          char c = client.read();             // lese das Byte, und dann
          Serial.write(c);                    // gebe es auf dem seriellen Monitor aus
          yield();
          HTTP_header += c;
          if (c == '\n') {                    // wenn das Byte eine Neue-Zeile Char ist
            // wenn die aktuelle Zeile leer ist, kamen 2 in folge.
            // dies ist das Ende der HTTP-Anfrage vom Client, also senden wir eine Antwort:
            if (currentLine.length() == 0) {
              // HTTP-Header fangen immer mit einem Response-Code an (z.B. HTTP/1.1 200 OK)
              // gefolgt vom Content-Type damit der Client weiss was folgt, gefolgt von einer Leerzeile:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              
              sDummy = PickValue(HTTP_header, "SSID");
              sDummy = HTML_RemoveSpecialChar(sDummy);
              sDummy.toCharArray(cWifi_Name, sDummy.length()+1);
              if (sDummy.length() < 1) bValidInputLength01 = false; else bValidInputLength01 = true;
              bValidInput01 = HTML_noSpecialChar(sDummy);             // Überprüfung auf verbotene Sonderzeichen bei der SSID

              sDummy = PickValue(HTTP_header, "PASSPHRASE");
              sDummy.toCharArray(cWifi_Password, sDummy.length()+1);
              if (sDummy.length() < 8) bValidInputLength02 = false; else bValidInputLength02 = true;
              bValidInput02 = HTML_noSpecialChar(sDummy);             // Überprüfung auf Sonderzeichen
              
              sDummy = PickValue(HTTP_header, "TR064_USER");
              sDummy.toCharArray(cTR064_User, sDummy.length()+1);
              if (sDummy.length() == 0) bValidInputLength03 = false; else bValidInputLength03 = true;
              bValidInput03 = HTML_noSpecialChar(sDummy);             // Überprüfung auf Sonderzeichen

              sDummy = PickValue(HTTP_header, "TR064_PASSWORD");
              sDummy.toCharArray(cTR064_Password, sDummy.length()+1);
              if (sDummy.length() < 8) bValidInputLength04 = false; else bValidInputLength04 = true;
              bValidInput04 = HTML_noSpecialChar(sDummy);             // Überprüfung auf Sonderzeichen
              
//##########################################################
              Serial.begin(115200);
              sDummy = PickValue(HTTP_header, "DL_SPEED");
              Serial.println();
              Serial.print("Download Speed: ");
              Serial.println(sDummy);
              ulDL_Speed = abs(sDummy.toInt());
              ulDL_Speed = ulDL_Speed * 1000;
              Serial.print("Download Speed: ");
              Serial.println(ulDL_Speed);
              Serial.println();
              sDummy.toCharArray(cDL_Speed, sDummy.length()+1);

              sDummy = PickValue(HTTP_header, "UL_SPEED");
              ulUL_Speed = abs(sDummy.toInt());
              ulUL_Speed = ulUL_Speed * 1000;
              sDummy.toCharArray(cUL_Speed, sDummy.length()+1);      

               
//##########################################################
                  

              sDummy = PickValue(HTTP_header, "ACTION");
              iDummy = sDummy.toInt();
              if (iDummy == 0) sMessage = "";
              else{
                bSaveToEEPROM = false;
                if (bValidInput01 && bValidInput02 && bValidInput03 && bValidInput04) bValidInput = true; else bValidInput = false;
                if (bValidInputLength01 && bValidInputLength02 && bValidInputLength03 && bValidInputLength04) bValidInputLength = true; else bValidInputLength = false;  
                if (bValidInputLength == false) sMessage = "<font color=red><strong>Unvollst&auml;ndige Eingabe!</strong></font>";
                else if (bValidInput == false) sMessage = "<font color=red><strong>Ung&uuml;tige Zeichen gefunden!</strong></font>";
                else{
                  bSaveToEEPROM = true;
                  sMessage = "<font color=green><strong>Werte wurden gespeichert!</strong></font>";
                }
              }
 
              client.println("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//DE\">");
              client.println("<html>");
              client.println("<head>");
              client.println("<link rel=\"shortcut icon\" href=\"data:image/x-icon;base64,");
              yield();
      			  client.println(String(Site_Picture));                           // Webpage Logo 16x16px
              yield();
              client.println("\" type=\"image/x-icon\" />");
              client.println("<title>Web Monitor</title>");
              client.println("</head>");
		  
      			  client.println("<body bgcolor=\"#adcede\">");
              client.println("<font color=\"#000000\" face=\"VERDANA,ARIAL,HELVETICA\">");
              client.println("<h1>");
              client.println("<img src=\"data:image/png;base64,");
              yield();
      			  client.println(String(WebMonitor_Picture));               // "Web Monitor" Picture 150x150px
              yield();
              client.println("\" hspace=10 align=\"middle\">");
              client.println("Web Monitor");
      			  client.println("</h1>");
              
              if (bShowFullSite == true){             
                client.println("<h2>Werte eingeben:</h2>");
                client.println("<form>");
                client.println("<table>");
                client.println("<colgroup>");
                client.println("<col width=\"20\">");
                client.println("<col width=\"180\">");
                client.println("<col width=\"300\">");
  			        client.println("<col width=\"150\">");
                client.println("</colgroup>");
  
        			  client.println("<tr>");
        			  client.println("<td></td>");
                client.println("<td><b>WLAN Name</b></td>");
                client.println("<td>");
                client.println("<input type=\"text\" style= \"width:250px\" name=\"SSID\" maxlength=\"32\" Value =\"");
                client.println(cWifi_Name);                              // Text-String des Eingebefeldes "WLAN Name" der Website
        			  client.println("\">");
        			  client.println("</td>");
                client.println("</tr>");
              		
        			  client.println("<tr>");
        			  client.println("<td></td>");
                client.println("<td><b>WLAN Passwort</b></td>");
                client.println("<td>");
                client.println("<input type=\"text\" style= \"width:250px\" name=\"PASSPHRASE\" maxlength=\"64\" Value =\"");
                client.println(cWifi_Password);                          // Text-String des Eingebefeldes "WLAN Passwort" der Website
        			  client.println("\">");
        			  client.println("</td>");
                client.println("</tr>");
              
         			  client.println("<tr>");
        			  client.println("<td></td>");
                client.println("<td><b>TR-064 User</b></td>");
                client.println("<td>");
                client.println("<input type=\"text\" style= \"width:250px\" name=\"TR064_USER\" maxlength=\"64\" Value =\"");
                client.println(cTR064_User);                             // Text-String des Eingebefeldes "TR-064 User" der Website
        			  client.println("\">");
        			  client.println("</td>");
                client.println("</tr>");

                 client.println("<tr>");
                client.println("<td></td>");
                client.println("<td><b>TR-064 Passwort</b></td>");
                client.println("<td>");
                client.println("<input type=\"text\" style= \"width:250px\" name=\"TR064_PASSWORD\" maxlength=\"64\" Value =\"");
                client.println(cTR064_Password);                         // Text-String des Eingebefeldes "TR-064 Passwort" der Website
                client.println("\">");
                client.println("</td>");
                client.println("</tr>");
                
                client.println("<tr>");
                client.println("<td></td>");
                client.println("<td><b>Download Speed</b></td>");
                client.println("<td>");
                client.println("<input type=\"text\" style= \"width:250px\" name=\"DL_SPEED\" maxlength=\"8\" Value =\"");
                client.println(cDL_Speed);                               // Text-String des Eingebefeldes "Download Speed" der Website
                client.println("\">");
                client.println("</td>");
                client.println("<td><b>MBit/s</b></td>");
                client.println("</tr>");
                
                client.println("<tr>");
                client.println("<td></td>");
                client.println("<td><b>Upload Speed</b></td>");
                client.println("<td>");
                client.println("<input type=\"text\" style= \"width:250px\" name=\"UL_SPEED\" maxlength=\"8\" Value =\"");
                client.println(cUL_Speed);                               // Text-String des Eingebefeldes "Upload Speed" der Website
                client.println("\">");
                client.println("</td>");
                client.println("<td><b>MBit/s</b></td>");
                client.println("</tr>");
              
        			  client.println("<tr>");
        			  client.println("<td></td>");
        			  client.println("<td></td>");
                client.println("<td>" + sMessage + "</td>");
                client.println("<td>");
                client.println("<button style=\"BACKGROUND-COLOR: yellow; width:200px; height:50px; border-radius:10px; border-width:3px;\" name=\"ACTION\" value=\"1\">");
        			  client.println("<strong>Alle Werte &uuml;bernehmen</strong> </button>");
        			  client.println("</td>");
                client.println("</tr>");
                
                client.println("</table>");

                yield();
        			  client.println(String(ExplanationText));                        // Webpage Erklärungstext zu Einstellungen
                yield();
                Serial.println("#############");
                client.println("</form>");
              }              
              client.println("</body");
			        client.println("</html>");
  
              // Die HTTP-Antwort wird mit einer Leerzeile beendet
              client.println();
              yield();

              // und wir verlassen mit einem break die Schleife
              break;
            } 
            else { // falls eine neue Zeile kommt, lösche die aktuelle Zeile
              currentLine = "";
            }
          } else if (c != '\r') {  // wenn etwas kommt was kein Zeilenumbruch ist,
            currentLine += c;      // füge es am Ende von currentLine an
          }
        }
      }
      // Die Header-Variable für den nächsten Durchlauf löschen
      HTTP_header = "";
      // Die Verbindung schließen
      client.stop();
      Serial.println("Client getrennt.");
      Serial.println("");
      yield();
      
      // eingelesene Einstellungen im EEPROM speichern
      if(bSaveToEEPROM == true){
        ESP.wdtFeed();
        WriteEEPROM();
        bSaveToEEPROM = false;
      }
    }
}

/*******************************************************************
 * Routine, um aus einem HHTP Rückgabestring die Werte aus eingebefeldern herauszufiltern
 * Übergabewerte: HTML-String und der zu suchende Schlüssel-String
 * Rückgabewert: String des zugehörigen Inhalts des vorgewählten Schlüssel-String
 *******************************************************************/
String ESP_Settings::PickValue(String HTML_String, String Search_String)
{
  if (HTML_String.indexOf("TR064_USER") >= 0) {
    int iHeaderPos = HTML_String.indexOf(Search_String);
    int iStartPos = HTML_String.indexOf("=", iHeaderPos) + 1;
    int iEndPos = HTML_String.indexOf("&", iHeaderPos);
    String sValue = HTML_String.substring(iStartPos, iEndPos);
    return sValue;
  }
  return "";
}

/*******************************************************************
 * Routine zur Überprüfung ob ASCII Sonderzeichen mit einem Wert > 127 in einem HTML String enthalten sind
 * Übergabewert: String mit zu überprüfenden ASCII Zeichen
 * Rückgabewert: True, wenn nur Zeichen 48..127  - False, wenn in Zeichen deutsche Umlaute enthalten sind
 *******************************************************************/
boolean ESP_Settings::HTML_noSpecialChar(String HTML2test)
{
  int iSpecialCharPos;
  iSpecialCharPos = HTML2test.indexOf("%");                           // find special ASCII sign
  if(iSpecialCharPos >= 0) return false;                              // return 'false' if a special ASCII sign is found
  return true;                                                        // return 'true' if no special ASCII signs are found
}

/*******************************************************************
 * Routine zur Überprüfung ob ASCII Sonderzeichen mit einem Wert > 127 in einem HTML String enthalten sind
 * Übergabewert: String mit zu überprüfenden ASCII Zeichen
 * Rückgabewert: True, wenn nur Zeichen 20..127  - False, wenn in Zeichen deutsche Umlaute enthalten sind
 *******************************************************************/
String ESP_Settings::HTML_RemoveSpecialChar(String HTML2test)
{
  String sQuotationMarks = (String)char(34);
  String sBackSlash = (String)char(92);
  
  HTML2test.replace("+", " ");                                 // Alle relevanten Sonderzeichen suchen und ersetzen
  HTML2test.replace("%20", " ");
  HTML2test.replace("%21", "!");
  HTML2test.replace("%22", sQuotationMarks);
  HTML2test.replace("%23", "#");
  HTML2test.replace("%24", "$");
  HTML2test.replace("%25", "%");
  HTML2test.replace("%26", "&");
  HTML2test.replace("%27", "'");
  HTML2test.replace("%28", "(");
  HTML2test.replace("%29", ")");
  HTML2test.replace("%2A", "*");
  HTML2test.replace("%2B", "+");
  HTML2test.replace("%2C", ",");
  HTML2test.replace("%2D", "-");
  HTML2test.replace("%2E", ".");
  HTML2test.replace("%2F", "/");
  HTML2test.replace("%3A", ":");
  HTML2test.replace("%3B", ";");
  HTML2test.replace("%3C", "<");
  HTML2test.replace("%3D", "=");
  HTML2test.replace("%3E", ">");
  HTML2test.replace("%3F", "?");
  HTML2test.replace("%40", "@");
  HTML2test.replace("%5B", "[");
  HTML2test.replace("%5C", sBackSlash);
  HTML2test.replace("%5D", "]");
  HTML2test.replace("%5E", "^");
  HTML2test.replace("%5F", "_");
  HTML2test.replace("%60", "`");
  HTML2test.replace("%7B", "{");
  HTML2test.replace("%7C", "|");
  HTML2test.replace("%7D", "}");
  HTML2test.replace("%7E", "~");
  return HTML2test;                                                   // return String without special characters
}

/*******************************************************************
 * Routine zur Umwandlung von UTF-8 kodierten Sonderzeichen in ASCII Zeichen
 * Übergabewert: String mit möglichen UTF-8 kodierten Zeichen
 * Rückgabewert: String mit zu ASCII umgewandelten Zeichen
 * ACHTUNG: Es werden 'echte' 1-Byte ASCII-Zeichen benutzt. Arduino nutzt für Umlaute normalerweise eine 2-Byte Codierung.
 *******************************************************************/
String ESP_Settings::UTF8toASCII(String sUTF8)
{
  String sHex;
  String sChar;
  byte bAscDezimal;
  int iSpecialCharPos;
  iSpecialCharPos = sUTF8.indexOf("%");                               // find special ASCII sign
  while(iSpecialCharPos >= 0){                                        // do while more special ASCII signs are found
    sHex = sUTF8.substring(iSpecialCharPos + 1, iSpecialCharPos + 3); // isolate the Hex-String for the ASCII sign
    bAscDezimal = (byte)strtoul(sHex.c_str(), NULL, 16);              // convertin Hex-String to decimal Byte value
    sChar = char(bAscDezimal);                                        // ASCII character to string
    sUTF8.replace("%" + sHex, sChar);                                 // replace special ASCII sign
    iSpecialCharPos = sUTF8.indexOf("%");                             // search for next special ASCII sign
  }
  return sUTF8;
}

/*******************************************************************
 * Routine zur Umwandlung von ASCII Sonderzeichen in UTF-8 kodierte Zeichen
 * Übergabewert: String mit möglichen Sonderzeichen im ASCII Format umgewandelten Zeichen String mit möglichen UTF-8 kodierten Zeichen
 * Rückgabewert: String mit umgewandelten UTF-8 kodierten Zeichen
 * ACHTUNG: Es werden 'echte' 1-Byte ASCII-Zeichen benutzt. Arduino nutzt für Umlaute normalerweise eine 2-Byte Codierung.
 *******************************************************************/
String ESP_Settings::ASCIItoUTF8(String ASCII_String)
{
  String sUnicode;
  String sFirstStr;
  String sSecondStr;
  char   cASCII;
  byte   bASCII;
   
  for (int i = ASCII_String.length(); i >= 0; i--){                   // search for character backwards in the string
    cASCII = ASCII_String.charAt(i);                                  // read selected character
    bASCII = (byte)cASCII;                                            // ASCII decimal value of the selected character
    if (bASCII > 127){                                                // if ASCII decimal value = special character
      sUnicode = String(bASCII, HEX);                                 // convert decimal to hex-string
      sUnicode.toUpperCase();                                         // comvert string to uppercase
      sUnicode = "&#" + sUnicode + ";";                               // create unicode string
      sFirstStr = ASCII_String.substring(0, i);                       // extract string up to the selected character
      sSecondStr = ASCII_String.substring(i+1);                       // extract string from the selected character to the end
      ASCII_String = sFirstStr + sUnicode + sSecondStr;               // build the new complete string
    }
    return ASCII_String;
  }
}

/*******************************************************************
 * Routine zum Löschen der definierten 512 Byte des internen EEPROM
 *******************************************************************/
void ESP_Settings::DeleteEEPROM(){
  char cDummy32[33] = "";
  char cDummy64[65] = "";
  byte bDummy04[4] = {0, 0, 0, 0};
  
  for (int i=0; i<33; ++i) cDummy32[i] = '\n';
  for (int i=0; i<65; ++i) cDummy64[i] = '\n';
  EEPROM.begin(512);
  EEPROM.put(0x0000, cDummy32);
  delay(200);
  EEPROM.put(0x0040, cDummy64);
  delay(200);
  EEPROM.put(0x00C0, cDummy64);
  delay(200);
  EEPROM.put(0x0140, cDummy64);
  delay(200);
  EEPROM.put(0x01C0, bDummy04);
  delay(200);
  EEPROM.put(0x01D0, bDummy04);
  delay(200);
  EEPROM.commit();            // Only needed for ESP8266 to get data written
  EEPROM.end();               // Free RAM copy of structure 
}

/*******************************************************************
 * Routine zum Schreiben aller Eingebewerte ins interne EEPROM
 *******************************************************************/
void ESP_Settings::WriteEEPROM(){
  EEPROM.begin(512);
  EEPROM.put(0x0000, cWifi_Name);
  delay(200);
  EEPROM.put(0x0040, cWifi_Password);
  delay(200);
  EEPROM.put(0x00C0, cTR064_User);
  delay(200);
  EEPROM.put(0x0140, cTR064_Password);
  delay(200);
  bDL_Speed[0] = (byte)((ulDL_Speed >> 24) & 0xFF) ;
  bDL_Speed[1] = (byte)((ulDL_Speed >> 16) & 0xFF) ;
  bDL_Speed[2] = (byte)((ulDL_Speed >> 8) & 0XFF);
  bDL_Speed[3] = (byte)((ulDL_Speed & 0XFF));
  EEPROM.put(0x01C0, bDL_Speed);
  delay(200);
  bUL_Speed[0] = (byte)((ulUL_Speed >> 24) & 0xFF) ;
  bUL_Speed[1] = (byte)((ulUL_Speed >> 16) & 0xFF) ;
  bUL_Speed[2] = (byte)((ulUL_Speed >> 8) & 0XFF);
  bUL_Speed[3] = (byte)((ulUL_Speed & 0XFF));
  EEPROM.put(0x01D0, bUL_Speed);
  delay(200);
  EEPROM.commit();            // Only needed for ESP8266 to get data written
  EEPROM.end();               // Free RAM copy of structure
}

/*******************************************************************
 * Routine zum Auslesen des WLAN Namens aus dem EEPROM
 *******************************************************************/
String ESP_Settings::Read_WifiSSID(void)
{
  EEPROM.begin(512);
  EEPROM.get(0x0000, cWifi_Name);
  EEPROM.end();
  return String(cWifi_Name);
}

/*******************************************************************
 * Routine zum Auslesen des WLAN Passwortes aus dem EEPROM
 *******************************************************************/
String ESP_Settings::Read_WifiPassword(void)
{
  EEPROM.begin(512);
  EEPROM.get(0x0040, cWifi_Password);
  EEPROM.end();
  return String(cWifi_Password);
}

/*******************************************************************
 * Routine zum Auslesen des TR-064 Users aus dem EEPROM
 *******************************************************************/
String ESP_Settings::Read_TR064_User(void)
{
  EEPROM.begin(512);
  EEPROM.get(0x00C0, cTR064_User);
  EEPROM.end();
  return String(cTR064_User);
}

/*******************************************************************
 * Routine zum Auslesen des TR-064 Passwortes aus dem EEPROM
 *******************************************************************/
String ESP_Settings::Read_TR064_Password(void)
{
  EEPROM.begin(512);
  EEPROM.get(0x0140, cTR064_Password);
  EEPROM.end();
  return String(cTR064_Password);
}

/*******************************************************************
 * Routine zum Auslesen der max. Download Geschwindigkeit in kByte/s
 *******************************************************************/
unsigned long ESP_Settings::Read_Download_Speed(void)
{
  EEPROM.begin(512);
  EEPROM.get(0x01C0, bDL_Speed);
  EEPROM.end();
  ulDL_Speed = (((unsigned long)bDL_Speed[0] << 24) 
              + ((unsigned long)bDL_Speed[1] << 16) 
              + ((unsigned long)bDL_Speed[2] << 8) 
              + ((unsigned long)bDL_Speed[3] ));
  return ulDL_Speed;
}

/*******************************************************************
 * Routine zum Auslesen der max. Upload Geschwindigkeit in kByte/s
 *******************************************************************/
unsigned long ESP_Settings::Read_Upload_Speed(void)
{
  EEPROM.begin(512);
  EEPROM.get(0x01D0, bUL_Speed);
  EEPROM.end();
  ulUL_Speed = (((unsigned long)bUL_Speed[0] << 24) 
              + ((unsigned long)bUL_Speed[1] << 16) 
              + ((unsigned long)bUL_Speed[2] << 8) 
              + ((unsigned long)bUL_Speed[3] ));
  return ulUL_Speed;
}
