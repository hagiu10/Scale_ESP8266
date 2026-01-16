#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H

#include <commonLibs.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <webServer.h>
#include <micro_sd.h>


extern AsyncWebSocket ws; // Create a WebSocket object

// Function prototypes
class webSocket: private micro_sd {
  public:
    webSocket();
    static void init(void);
    static void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len);
};
#endif // WEB_SOCKET_H
