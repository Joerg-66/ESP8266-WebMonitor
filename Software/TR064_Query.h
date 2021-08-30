/*
 *
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
