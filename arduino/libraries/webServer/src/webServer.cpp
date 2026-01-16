#include <webServer.h>

AsyncWebServer server = AsyncWebServer(80);  // Create an instance of the AsyncWebServer class

/** Constructor
 */
webServer::webServer(){
    // server = new AsyncWebServer(80);  // Port 80 is the default port for HTTP
}
/** Initialize the web server
 */
void webServer::init(void) {
    boolean result =  WiFi.softAP("ESP8266", "123456789");
#ifdef DEBUG
    if (result) {
        Serial.printf("webServer::init - WiFi soft access point is running. [%lu ms]\n", millis());
    } else {
        Serial.printf("webServer::init - WiFi soft access point failed. [%lu ms]\n", millis());
    }
#endif
    IPAddress local_IP(192, 168, 0, 10);  // Set your desired IP address
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    result = WiFi.softAPConfig(local_IP, gateway, subnet);
#ifdef DEBUG
    if (result) {
        Serial.printf("webServer::init - AP Config Success. [%lu ms]\n", millis());
    } else {
        Serial.printf("webServer::init - AP Config Failed. [%lu ms]\n", millis());
    }
#endif
}
/** Load the web page
 */
void webServer::loadWebPage(void) {
    // Load the web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", webPage_html);
    });
    server.begin(); 
#ifdef DEBUG
    Serial.printf("webServer::loadWebPage - Web server is running. [%lu ms]\n", millis());
#endif
}
