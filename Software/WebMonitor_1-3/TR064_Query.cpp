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

 
#include <Arduino.h>
#include <tr064.h>
#include "TR064_Query.h"


TR064_Query::TR064_Query(int ROUTER_PORT, String ROUTER_IP, String USER, String PASSWORD) : TR064(ROUTER_PORT, ROUTER_IP, USER, PASSWORD){};


void TR064_Query::refreshNonce(void){
  // Do a dummy request which will result in an authentication error, but will refresh the nonce
  String req[][2] = {{"NewAssociatedDeviceIndex", "1"}};
  action("urn:dslforum-org:service:WLANConfiguration:1", "GetGenericAssociatedDeviceInfo", req, 1);
}

int TR064_Query::getDeviceNumber(void) {
  String params[][2] = {{}};
  String req[][2] = {{"NewHostNumberOfEntries", ""}};
  action("urn:dslforum-org:service:Hosts:1", "GetHostNumberOfEntries", params, 0, req, 1);
  int numDev = (req[0][1]).toInt();
  return numDev;
}

float TR064_Query::getUpstreamMaxBitRate(void) {
  String params[][2] = {{}};
  String req[][2] = {{"NewLayer1UpstreamMaxBitRate", ""}};
  action("urn:dslforum-org:service:WANCommonInterfaceConfig:1", "GetCommonLinkProperties", params, 0, req, 1);
  long numDev = req[0][1].toInt();
  //Serial.println("MaxUpload: " + String(numDev / factor2));
  return (numDev);
}

float TR064_Query::getDownstreamMaxBitRate(void) {
  String params[][2] = {{}};
  String req[][2] = {{"NewLayer1DownstreamMaxBitRate", ""}};
  action("urn:dslforum-org:service:WANCommonInterfaceConfig:1", "GetCommonLinkProperties", params, 0, req, 1);
  long numDev = (req[0][1]).toInt();
  //Serial.println ("MaxDownload: " + String(numDev / factor2));
  return (numDev);
}

float TR064_Query::getNewByteUploadRate(void) {
  String params[][2] = {{"NewSyncGroupIndex", "0"}};
  String req[][2] = {{"Newus_current_bps", ""}};
  action("urn:dslforum-org:service:WANCommonInterfaceConfig:1", "X_AVM-DE_GetOnlineMonitor",params, 1, req, 1);
  long numDev = req[0][1].toInt();
  //Serial.println("Upload: " + String((numDev*8) / factor1));
  return ((numDev*8) / 1024);
}

float TR064_Query::getNewByteDownloadRate(void) {
  String params[][2] = {{"NewSyncGroupIndex", "0"}};
  String req[][2] = {{"Newds_current_bps", ""}};
  action("urn:dslforum-org:service:WANCommonInterfaceConfig:1", "X_AVM-DE_GetOnlineMonitor",params, 1, req, 1);
  long numDev = req[0][1].toInt();
  //Serial.println("Download: " + String((numDev*8) / factor1));
  return ((numDev*8) / 1024);
}

//*** Überprüfung ob der Router eine WAN-PPP Verbindung oder eine WAN-IP Verbindung nutzt. ***
String TR064_Query::getDefaultConnectionService(void) {
  String params[][2] = {{"NewDefaultConnectionService", ""}};
  String req[][2] = {{"NewDefaultConnectionService", "DefaultConnectionService"}};
  action("urn:dslforum-org:service:Layer3Forwarding:1", "GetDefaultConnectionService", params, 1, req, 1);
  String sDefaultConnectionService = req[0][1];
  return (sDefaultConnectionService);
}


//*** Der Router nutzt eine WAN-PPP Verbindung. Typisch für Kabel-Verbindungen. ***
String TR064_Query::getWANPPPConnectionStatus(void) {
  String params[][2] = {{"NewConnectionStatus", ""}};
  String req[][2] = {{"NewConnectionStatus", "ConnectionStatus"}};
  action("urn:dslforum-org:service:WANPPPConnection:1", "GetStatusInfo", params, 1, req, 1);
  String sConnectionStatus = req[0][1];
  return (sConnectionStatus);
}


//*** Der Router nutzt eine WAN-IP Verbindung. Typisch für DSL-Verbindungen. ***
String TR064_Query::getWANIPConnectionStatus(void) {
  String params[][2] = {{"NewConnectionStatus", ""}};
  String req[][2] = {{"NewConnectionStatus", "ConnectionStatus"}};
  action("urn:dslforum-org:service:WANIPConnection:1", "GetStatusInfo", params, 1, req, 1);
  String sConnectionStatus = req[0][1];
  return (sConnectionStatus);
}


String TR064_Query::getNewLinkStatus(void) {
  String params[][2] = {{"NewLinkType",""},{"NewLinkStatus", ""}};
  String req[][2] = {{"NewLinkType","LinkType"},{"NewLinkStatus", "LinkStatus"}};
  action("urn:dslforum-org:service:WANDSLLinkConfig:1", "GetDSLLinkInfo", params, 2, req, 2);
  String sLinkType = req[0][1];
  String sLinkStatus = req[1][1];
  return (sLinkType);
}


void TR064_Query::GuestWlanOn(void) {
  // (Re-) Initialize the TR-064 library - it is done every time, as maybe the connection has lost before
  // TODO: This should be fixed with b33fbbc6, we need to check if that did the job.
  init();
  String params[][2] = {{"NewEnable", "1"}};
  String req[][2] = {{}};
  action("urn:dslforum-org:service:WLANConfiguration:3", "SetEnable", params, 1, req, 2);
  Serial.println ("Gastnetzwerk Ein");
}

void TR064_Query::GuestWlanOff(void) {
  // (Re-) Initialize the TR-064 library - it is done every time, as maybe the connection has lost before
  // TODO: This should be fixed with b33fbbc6, we need to check if that did the job.
  init();
  String params[][2] = {{"NewEnable", "0"}};
  String req[][2] = {{}};
  action("urn:dslforum-org:service:WLANConfiguration:3", "SetEnable", params, 1, req, 2);
  Serial.println("Gastnetzwerk Aus");
}

//*** Überprüfen ob das Gast-WLAN eingeschaltet ist ***
int TR064_Query::getGuestWlanEnabled(void) {
  String params[][2] = {"NewEnable", ""};
  String req[][2] = {"NewEnable", "Enable"};
  action("urn:dslforum-org:service:WLANConfiguration:3", "GetInfo", params, 1, req, 1);
  int iEnabled = req[0][1].toInt();;
  return (iEnabled);
}

//*** Überprüfen ob das Gast-WLAN aktiv ist ***
String TR064_Query::getGuestWlanStatus(void) {
  String params[][2] = {{"NewStatus", ""}};
  String req[][2] = {{"NewStatus", "Status"}};
  action("urn:dslforum-org:service:WLANConfiguration:3", "GetInfo", params, 1, req, 1);
  String sStatus = req[0][1];
  return (sStatus);
}
