#include <commonLibs.h>
#include <webServer.h>
#include <webSocket.h>
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
  // webServer::init();
  // webServer::loadWebPage();
  // webSocket::init();
  micro_sd::init();
  hx711_ic::init();
  rtc::init();
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
  static unsigned long last = 0; 
  if (millis() - last > 5000) {  
    last = millis(); 
    float weight = hx711_ic::readWeight(); 
    String date_time = rtc::getDateTime("year") + "-" + rtc::getDateTime("month") + "-" + rtc::getDateTime("day") + " " +
                       rtc::getDateTime("hour") + ":" + rtc::getDateTime("minute") + ":" + rtc::getDateTime("second");
    micro_sd::writeData(weight, "day", date_time);
#ifdef DEBUG
    Serial.printf("main::loop - Date Time: %s \n", date_time.c_str());
#endif 
  }
}