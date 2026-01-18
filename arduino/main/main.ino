#include <commonLibs.h>
#include <webServer.h>
#include <webSocket.h>
#include <micro_sd.h>
#include <hx711_ic.h>


// the setup function runs once when you press reset or power the board
void setup() {
#ifdef DEBUG
  // Serial port for debugging purposes
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("Starting...\n");
#endif
  // voltageMonitor.init();
  webServer::init();
  webServer::loadWebPage();
  // webSocket::init();
  // micro_sd::init();
  // hx711_ic::init();
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
  static unsigned long last = 0; 
  if (millis() - last > 60000) {  
    last = millis(); 
    // float weight = hx711_ic::readWeight();
    printf("Looping... [%lu ms]\n", millis()); 
  }
}
