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
  Serial.println("Starting setup...\n");
#endif
  rtc::init();
  webServer::init();
  micro_sd::init();
  hx711_ic::init();
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
  // dnsServer.processNextRequest();
  static unsigned long last = 0; 
  if (millis() - last > 3000) {  
    last = millis(); 
    float weight = hx711_ic::readWeight(); 
    micro_sd::writeData((weight*1000)/1000.0); // Write weight to SD card with three decimal places (adjust as needed);
  }
}