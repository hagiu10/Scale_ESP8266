#include <webSocket.h>

AsyncWebSocket ws = AsyncWebSocket("/ws");  // Create an instance of the AsyncWebSocket class

/** Constructor
 */
webSocket::webSocket() {
    // ws = new AsyncWebSocket("/ws");  // Create an instance of the AsyncWebSocket class
}
/** Initialize the web socket
 */
void webSocket::init(void) {
    // Initialize WebSocket server
    ws.onEvent(webSocket::onWsEvent);
    server.addHandler(&ws);
#ifdef DEBUG
    Serial.printf("webSocket::init - webSocket initialized. [%lu ms]\n", millis());
#endif
}
/** Handle the WebSocket event
 */
void webSocket::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) { 
        #ifdef DEBUG
        Serial.printf("WS client %u connected\n", client->id()); 
        #endif
    } else if (type == WS_EVT_DATA) { 
        String msg; 
        for (size_t i = 0; i < len; i++){
            msg += (char)data[i];
        } 
        if (msg == "getData") { 
            String json = micro_sd::loadDataFromSD(); 
            client->text(json); 
        } 
        } else if (type == WS_EVT_DISCONNECT) { 
            #ifdef DEBUG
            Serial.printf("WS client %u disconnected\n", client->id());
            #endif 
        }
}
