#include <commonLibs.h>
#include <webServer.h>
#include <micro_sd.h>
#include <hx711_ic.h>
#include <rtc.h>
#include <create_littleFS_files.h>

#define REPEAT_READ_WEIGHT_INTERVAL 3e3 // Interval in milliseconds to read weight
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
  // rtc::init();
  // micro_sd::init();
  // hx711_ic::init();
  // LittleFSFile::init();
  #ifdef DEBUG
  Serial.printf("\nSetup complete. [%lu ms]\n", millis());
  #endif
  delay(100);
}

// the loop function runs over and over again forever
void loop() {
  handleButton();
  // webServer::handleAPTimeout(); // Check if we need to stop the AP due to timeout
  // handleReadWeight();
}
void handleButton() {
    static bool laststate = HIGH;
    bool currentState = digitalRead(BUTTON_PIN);
    if (currentState == LOW && laststate == HIGH) {
        //webServer::startAP();
        LittleFSFile::init();
        verify_littlefs();
        delay(200); // Debounce delay
    }
    laststate = currentState;
}
void handleReadWeight() {
    static unsigned long last = 0; 
    if (millis() - last > REPEAT_READ_WEIGHT_INTERVAL) {  
        last = millis();
        float weight = hx711_ic::readWeight(); 
        micro_sd::writeData((weight*1000)/1000.0); // Write weight to SD card with three decimal places (adjust as needed);
    }
}
void verify_littlefs() {
  FSInfo info;
  LittleFS.info(info);

  Serial.printf("LittleFS total bytes: %u\n", info.totalBytes);
  Serial.printf("LittleFS used bytes: %u\n", info.usedBytes);
  Serial.printf("LittleFS block size: %u\n", info.blockSize);
  Serial.printf("LittleFS page size: %u\n", info.pageSize);

  // Adresa de start = flash_size - fs_size
  uint32_t flash_size = ESP.getFlashChipSize();
  uint32_t fs_size = info.totalBytes;

  uint32_t fs_start = flash_size - fs_size;

  Serial.printf("LittleFS start address: 0x%06X\n", fs_start);

  // Read all files in the root directory
  Serial.println("\nFiles in root directory:");
  File root = LittleFS.open("/", "r");
  File fileEntry = root.openNextFile();
  while (fileEntry) {
      Serial.print(" - ");
      Serial.println(fileEntry.name());
      fileEntry = root.openNextFile();
  }
  root.close();
}