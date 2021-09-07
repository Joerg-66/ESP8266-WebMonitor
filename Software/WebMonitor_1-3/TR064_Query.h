/*
 * TR-064 Kommunikation mit einem Router (Sammlung von Befehlen mit Nutzung der TR-064 SOAP Library von René Vollmer)
 * 
 * Autor: Jörg Bernhardt
 * Datum 05/2021
 * 
 * wichtige Links:
 * https://github.com/Aypac/Arduino-TR-064-SOAP-Library
 * 
 */


#ifndef TR064_Query_h
#define TR064_Query_h

#include <Arduino.h>
#include <tr064.h>

class TR064_Query: public TR064
{
  public:
    TR064_Query(int ROUTER_PORT, String ROUTER_IP, String USER, String PASSWORD);

    void refreshNonce(void);
    int getDeviceNumber(void);
    float getUpstreamMaxBitRate(void);
    float getDownstreamMaxBitRate(void);
    float getNewByteUploadRate(void);
    float getNewByteDownloadRate(void);
    String getDefaultConnectionService(void);
    String getWANPPPConnectionStatus(void);
    String getWANIPConnectionStatus(void);
    String getNewLinkStatus(void);
    
    void GuestWlanOn(void);
    void GuestWlanOff(void);
    int getGuestWlanEnabled(void);
    String getGuestWlanStatus(void);    
};

#endif
