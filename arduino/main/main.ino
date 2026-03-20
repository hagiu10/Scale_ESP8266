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
  Serial.println("\n\nStarting setup...\n");
#endif
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with pull-up resistor
  rtc::init();
  micro_sd::init();
  hx711_ic::init();
  #ifdef DEBUG
  Serial.printf("\nSetup complete. [%lu ms]\n", millis());
  #endif
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
  handleButton();
  webServer::handleAPTimeout(); // Check if we need to stop the AP due to timeout
 
  // static unsigned long last = 0; 
  // if (millis() - last > 3000) {  
  //   last = millis(); 
  //   float weight = hx711_ic::readWeight(); 
  //   micro_sd::writeData((weight*1000)/1000.0); // Write weight to SD card with three decimal places (adjust as needed);
  // }
}
void handleButton() {
    static bool laststate = HIGH;
    bool currentState = digitalRead(BUTTON_PIN);
    if (currentState == LOW && laststate == HIGH) {
        webServer::startAP();
    }
    laststate = currentState;
}