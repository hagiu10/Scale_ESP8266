#include <commonLibs.h>
#include <webServer.h>
#include <micro_sd.h>
#include <hx711_ic.h>
#include <rtc.h>


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
  rtc::init();
  webServer::init();
  micro_sd::init();
  hx711_ic::init();
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
  static unsigned long last = 0; 
  if (millis() - last > 10000) {  
    last = millis(); 
    float weight = hx711_ic::readWeight(); 
    micro_sd::writeData(weight);
  }
}