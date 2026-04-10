#include <webServer.h>
   
AsyncWebServer* server = nullptr;  // Initialize the server pointer to nullptr
bool apActive = false;
unsigned long apStartTime = 0;

/** Constructor
 */
webServer::webServer(){
    // server = new AsyncWebServer(80);  // Port 80 is the default port for HTTP
}
/** Initialize the web server
 */
void webServer::init(void) {
    server = new AsyncWebServer(80);  // Create a new server instance on port 80
    WiFi.mode(WIFI_AP); // Set Wi-Fi mode to Access Point
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
    // if (!LittleFS.begin()) {
    //     #ifdef DEBUG
    //     Serial.printf("webServer::init - Failed to mount LittleFS. [%lu ms]\n", millis());
    //     #endif
    //     return;
    // }
    // loadChartJS();
    loadWebPage();
    initHandleDataEndpoint();
    setRTCtime();
    server->begin();
}
void webServer::startAP() {
    if (!apActive) {
        init();
        apStartTime = millis();
        apActive = true;
        pinMode(LED_PIN, OUTPUT);
        digitalWrite(LED_PIN, LOW); // Turn on the LED when AP is active
        #ifdef DEBUG
        Serial.printf("webServer::startAP - Access Point started. [%lu ms]\n", millis());
        #endif
    }
}
void webServer::stopAP() {
    if (apActive) {
        if (server != nullptr) {
            delete server; // Free the server instance
            server = nullptr; // Set the pointer to nullptr to avoid dangling pointer issues
            #ifdef DEBUG
            Serial.printf("webServer::stopAP - Server instance deleted. [%lu ms]\n", millis());
            #endif
        }
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_OFF);
        apActive = false;
        digitalWrite(LED_PIN, HIGH); // Turn off the LED when AP is stopped
        pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin back to input with pull-up
        #ifdef DEBUG
        Serial.printf("webServer::stopAP - Access Point stopped. [%lu ms]\n", millis());
        Serial.printf("webServer::stopAP - Only to stop AP is to reset software. [%lu ms]\n", millis());
        #endif
        ESP.restart(); // Restart the ESP to ensure a clean state after stopping the AP
    }
}
void webServer::handleAPTimeout() {
    if (!apActive) return; // If AP is not active, do nothing

    if (WiFi.softAPgetStationNum() > 0 ) {
        apStartTime = millis(); // Reset the AP start time if there are clients connected
        return; // If there are clients connected, do not stop the AP
    }
    if (millis() - apStartTime > 60000) { // 60 seconds timeout
        stopAP();
    }
}
// void webServer::loadChartJS() {
//     server->on("/chart.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
//         File file = LittleFS.open("/chart.min.js", "r");
//         if (!file) {
//             #ifdef DEBUG
//             Serial.printf("webServer::loadChartJS - Failed to open chart.min.js. [%lu ms]\n", millis());
//             #endif
//             return;
//         }
//         request->send(file, "application/javascript"); // Serve the JavaScript file with the correct MIME type
//         file.close();
//     }); 
// #ifdef DEBUG
//     Serial.printf("webServer::loadChartJS - Chart.js endpoint initialized. [%lu ms]\n", millis());
// #endif
// }
/** Load the web page
 */
void webServer::loadWebPage(void) {
    // Load the web page
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", webPage);
    }); 
#ifdef DEBUG
    Serial.printf("webServer::loadWebPage - Web server is running. [%lu ms]\n", millis());
#endif
}
/** Endpoint */
void webServer::initHandleDataEndpoint(void) {
  server->on("/data", HTTP_GET, handleDataRequest);
  #ifdef DEBUG
  Serial.printf("webServer::initHandleDataEndpoint - Data endpoint initialized. [%lu ms]\n", millis());
  #endif
}

void webServer::setRTCtime() {
    server->on("/setRTC", HTTP_POST, [](AsyncWebServerRequest *request){},
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