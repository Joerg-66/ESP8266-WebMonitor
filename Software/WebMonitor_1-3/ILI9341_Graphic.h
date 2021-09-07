/*
 * Ansteuerung eines Displays mit ILI9341-Controller und vorgefertigten Bildern zur Visualisierung
 * 
 * Autor: Jörg Bernhardt
 * Datum 11/2020
 * 
 * wichtige Links:
 * Bilder generieren:  https://javl.github.io/image2cpp/
 * 
 */

 
#ifndef ILI9341_Graphic_h
#define ILI9341_Graphic_h

#include <Arduino.h>
#include <Adafruit_GFX.h>       // include Adafruit graphics library
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library
  
class ILI9341_Graphic: public Adafruit_ILI9341
{
  private:
    uint16_t iMaxBitRate;

    
  public:
    ILI9341_Graphic(uint8_t CS, uint8_t RS, uint8_t RST);

    //void begin();

    void Setup();                                                               //Grundeinstellung durchführen
    
    uint16_t getColorValue(byte bRed, byte bGreen, byte bBlue);                 //Umwandlung 8-Bit RGB Werte zu uint16_t (RRRRRGGGGGGBBBBB) zur Darstellung auf dem Display
    
    void Grid_01(int xPos, int yPos, long currentValue, long maxValue, uint16_t barColor);  //Symbol: eines Gittersnetzes mit Verlaufshistorie 01 
    void Grid_02(int xPos, int yPos, long currentValue, long maxValue, uint16_t barColor);  //Symbol: eines Gittersnetzes mit Verlaufshistorie 02
    
    void TextBits(int xPos, int yPos, long lValue, uint16_t u16Color);          //Formatierung eines Zahlenwertes in ein besser lesbares Textformat

    void Connection(int xPos, int yPos, boolean bStatus);                       //Symbol: Netzwerkverbindung 
    
    void Wifi(int xPos, int yPos, uint16_t u16Color, String sDbValue = "");     // sDbValue ist ein optionales Argument
    void Cloud(int xPos, int yPos, uint16_t u16Color);                          //Symbol: Wolke
    void Computer(int xPos, int yPos, uint16_t u16Color);                       //Symbol: Computer 
    void ArrowUp(int xPos, int yPos, uint16_t u16Color);                        //Symbol: Pfeil nach oben 
    void ArrowUpTiny(int xPos, int yPos, uint16_t u16Color);                    //Symbol: kleiner Pfeil nach oben 
    void ArrowDown(int xPos, int yPos, uint16_t u16Color);                      //Symbol: Pfeil nach unten 
    void ArrowDownTiny(int xPos, int yPos, uint16_t u16Color);                  //Symbol: kleiner Pfeil nach unten
    void Router(int xPos, int yPos, boolean blnActive);                         //Symbol: Router
    void Earth(int xPos, int yPos, boolean blnActive);                          //Symbol: Erde
    void Flash(int xPos, int yPos, uint16_t u16Color);                          //Symbol: Blitz
    void FlashTiny(int xPos, int yPos, uint16_t u16Color);                      //Symbol: kleiner Blitz
    void Flash02(int xPos, int yPos, uint16_t u16Color);                        //Symbol: Blitz 02
    void Guest(int xPos, int yPos, uint16_t u16ColorF, uint16_t u16ColorB);     //Symbol: Personen
    void Logo(int xPos, int yPos);                                              //Symbol: Start-Logo
    
    void MessageScreen_01(String sIP_Address);                                  //Darstellung: Nachrichtenseite 01
    void MessageScreen_02(String sWifiSSID, String sWifiPassword, String sTR064_User, String sTR064_Password, String sMaxDL_Speed, String sMaxUL_Speed);  //Darstellung: Nachrichtenseite 02
    void MessageScreen_03(void);                                                //Darstellung: Nachrichtenseite 03
    void MessageScreen_04(void);                                                //Darstellung: Nachrichtenseite 04
};

#endif
