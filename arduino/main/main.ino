#include <commonLibs.h>
#include <webServer.h>
#include <webSocket.h>
#include <rtos.h>
#include <micro_sd.h>


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
  webSocket::init();
  rtos::init();
  micro_sd::init();
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
  rtos::executeTasks();
}
