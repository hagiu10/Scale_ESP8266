#include <webServer.h>

DNSServer dnsServer; // Create a DNS server for captive portal functionality    
AsyncWebServer server = AsyncWebServer(80);  // Create an instance of the AsyncWebServer class

/** Constructor
 */
webServer::webServer(){
    // server = new AsyncWebServer(80);  // Port 80 is the default port for HTTP
}
/** Initialize the web server
 */
void webServer::init(void) {
    boolean result =  WiFi.softAP("Cantar-Wi-Fi-1");
#ifdef DEBUG
    if (result) {
        Serial.printf("webServer::init - WiFi soft access point is running. [%lu ms]\n", millis());
    } else {
        Serial.printf("webServer::init - WiFi soft access point failed. [%lu ms]\n", millis());
    }
#endif
    IPAddress local_IP(192, 168, 0, 10);  // Set your desired IP address
    IPAddress gateway(192, 168, 0, 10);   // Set the gateway to the same IP for a captive portal
    IPAddress subnet(255, 255, 255, 0);
    result = WiFi.softAPConfig(local_IP, gateway, subnet);
#ifdef DEBUG
    if (result) {
        Serial.printf("webServer::init - AP Config Success. [%lu ms]\n", millis());
    } else {
        Serial.printf("webServer::init - AP Config Failed. [%lu ms]\n", millis());
    }
#endif
//     result = dnsServer.start(53, "*", local_IP);
// #ifdef DEBUG
//     if (result) {
//         Serial.printf("webServer::init - DNS server started successfully. [%lu ms]\n", millis());
//     } else {
//         Serial.printf("webServer::init - DNS server failed to start. [%lu ms]\n", millis());
//     }
// #endif
    loadWebPage();
    initHandleDataEndpoint();
    setRTCtime();
    // trikConectionInternet();
    server.begin();
}
/** Load the web page
 */
void webServer::loadWebPage(void) {
    // Load the web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", webPage);
    }); 
#ifdef DEBUG
    Serial.printf("webServer::loadWebPage - Web server is running. [%lu ms]\n", millis());
#endif
}
/** Endpoint */
void webServer::initHandleDataEndpoint(void) {
  server.on("/data", HTTP_GET, handleDataRequest);
  #ifdef DEBUG
  Serial.printf("webServer::initHandleDataEndpoint - Data endpoint initialized. [%lu ms]\n", millis());
  #endif
}
void webServer::trikConectionInternet() {
    // server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(204);
    // });

    // server.on("/gen_204", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(204);
    // });

    // server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(200, "text/html", "<html><body>OK</body></html>");
    // });

    // server.on("/captive.apple.com", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(200, "text/html", "<html><body>Success</body></html>");
    // });

    // server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(200, "text/plain", "OK");
    // });

    // server.on("/library/test/success.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(200, "text/plain", "OK");
    // });

}

void webServer::setRTCtime() {
    server.on("/setRTC", HTTP_POST, [](AsyncWebServerRequest *request){},
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    
    request->send(200, "text/plain", "Received RTC time data.");

    // Convertim body-ul în string
    String body;
    body.reserve(total);
    for (size_t i = 0; i < len; i++) {
      body += (char)data[i];
    }

    // Parsăm JSON-ul
    DynamicJsonDocument doc(256);
    DeserializationError err = deserializeJson(doc, body);

    if (err) {
        request->send(400, "text/plain", "JSON invalid");
        #ifdef DEBUG
        Serial.printf("webServer::setRTCtime - Invalid JSON received: %s. [%lu ms]\n", body.c_str(), millis());
        #endif
        return;
    }

    int year   = doc["year"];
    int month  = doc["month"];
    int day    = doc["day"];
    int hour   = doc["hour"];
    int minute = doc["minute"];
    int second = doc["second"];

    rtc::setDateTime(year, month, day, hour, minute, second);

    #ifdef DEBUG
    Serial.printf("webServer::setRTCtime - RTC time set. Received JSON: %s. [%lu ms]\n", body.c_str(), millis());
    #endif
});
#ifdef DEBUG
    Serial.printf("webServer::setRTCtime - RTC time set endpoint initialized. [%lu ms]\n", millis());
#endif
}
/** Handle the data request
 * This function will be called when the client requests the /data endpoint 
 * and will send the data from the SD card in JSON format
 */
void webServer::handleDataRequest(AsyncWebServerRequest *request) {
    if (request->hasParam("year") && request->hasParam("month") && request->hasParam("day")) {
        sendDataDay(request);
    } else if (request->hasParam("year") && request->hasParam("month")) {
        sendDataMonth(request);
    } else if (request->hasParam("year")) {
        sendDataYear(request);
    } else {
        request->send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    }
#ifdef DEBUG
    Serial.printf("webServer::handleDataRequest - Data request handled. [%lu ms]\n", millis());
#endif
}
/* Send data for a specific day */
void webServer::sendDataDay(AsyncWebServerRequest *request) {
    String json = micro_sd::getDataFromSD(request->getParam("year")->value(), request->getParam("month")->value(), request->getParam("day")->value());
    request->send(200, "application/json", json);
    #ifdef DEBUG
    Serial.printf("webServer::sendDataDay - JSON response:\n");    
    #endif
}
/* Send data for a specific month */
void webServer::sendDataMonth(AsyncWebServerRequest *request) {
    String json = micro_sd::getDataFromSD(request->getParam("year")->value(), request->getParam("month")->value());
    request->send(200, "application/json", json);
    #ifdef DEBUG
    Serial.printf("webServer::sendDataMonth - JSON response: %s\n", json.c_str());    
    #endif
}
/* Send data for a specific year */
void webServer::sendDataYear(AsyncWebServerRequest *request) {
    String json = micro_sd::getDataFromSD(request->getParam("year")->value());
    request->send(200, "application/json", json);
    #ifdef DEBUG  
    Serial.printf("webServer::sendDataYear - JSON response: %s\n", json.c_str());  
    #endif
}