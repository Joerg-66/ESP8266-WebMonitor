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

    void Setup();
    
    uint16_t getColorValue(byte bRed, byte bGreen, byte bBlue);
    
    void Grid_01(int xPos, int yPos, long currentValue, long maxValue, uint16_t barColor);
    void Grid_02(int xPos, int yPos, long currentValue, long maxValue, uint16_t barColor);
    
    void TextBits(int xPos, int yPos, long lValue, uint16_t u16Color);

    void Connection(int xPos, int yPos, boolean bStatus);
    
    void Wifi(int xPos, int yPos, uint16_t u16Color, String sDbValue = "");  // sDbValue ist ein optionales Argument
    void Cloud(int xPos, int yPos, uint16_t u16Color);
    void Computer(int xPos, int yPos, uint16_t u16Color);
    void ArrowUp(int xPos, int yPos, uint16_t u16Color);
    void ArrowUpTiny(int xPos, int yPos, uint16_t u16Color);
    void ArrowDown(int xPos, int yPos, uint16_t u16Color);
    void ArrowDownTiny(int xPos, int yPos, uint16_t u16Color);
    void Router(int xPos, int yPos, boolean blnActive);
    void Earth(int xPos, int yPos, boolean blnActive);
    void Flash(int xPos, int yPos, uint16_t u16Color);
    void FlashTiny(int xPos, int yPos, uint16_t u16Color);
    void Flash02(int xPos, int yPos, uint16_t u16Color);
    void Guest(int xPos, int yPos, uint16_t u16ColorF, uint16_t u16ColorB);
    void Logo(int xPos, int yPos);
    
    void MessageScreen_01(String sIP_Address);
    void MessageScreen_02(String sWifiSSID, String sWifiPassword, String sTR064_User, String sTR064_Password, String sMaxDL_Speed, String sMaxUL_Speed);
    void MessageScreen_03(void);
    void MessageScreen_04(void);
};

#endif
