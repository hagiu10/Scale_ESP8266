#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <commonLibs.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <micro_sd.h>
#include <rtc.h>
#include <ArduinoJson.h>
#include "../utils/webPage.h"


extern AsyncWebServer server;  // Create a server object on port 80

// Function prototypes
class webServer: micro_sd, rtc {
  public:
    webServer();
    static void init(void);
  private:
    static void loadWebPage(void);
    static void setRTCtime(void);
    static void initHandleDataEndpoint(void);
    static void handleDataRequest(AsyncWebServerRequest *request);
    static void sendDataDay(AsyncWebServerRequest *request);
    static void sendDataMonth(AsyncWebServerRequest *request);
    static void sendDataYear(AsyncWebServerRequest *request); 
};
#endif // WEB_SERVER_H
