#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <commonLibs.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include "../utils/webPage.h"


extern AsyncWebServer server;  // Create a server object on port 80

// Function prototypes
class webServer {
  public:
    webServer();
    static void init(void);
    static void loadWebPage(void);
};
#endif // WEB_SERVER_H
