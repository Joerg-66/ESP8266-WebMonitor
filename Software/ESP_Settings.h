/*
 * https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/server-examples.html
 * “Encode image to Base64” --> https://base64.guru/converter/encode/image
 * 
 * 
 */

#ifndef ESP_Settings_h
#define ESP_Settings_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

class ESP_Settings
{
  public:
    void WebInterface(boolean bShowFullSite);
    String Read_WifiSSID(void);
    String Read_WifiPassword(void);
    String Read_TR064_User(void);
    String Read_TR064_Password(void);
    unsigned long Read_Download_Speed(void);
    unsigned long Read_Upload_Speed(void);
    void DeleteEEPROM(void);
    
  private:
    boolean HTML_noSpecialChar(String HTML2test);
    String HTML_RemoveSpecialChar(String HTML2test);
    String UTF8toASCII(String sUTF8);
    String ASCIItoUTF8(String ASCII_String);
    String PickValue(String HTML_String, String Search_String);
    void WriteEEPROM(void);
        
};

#endif
