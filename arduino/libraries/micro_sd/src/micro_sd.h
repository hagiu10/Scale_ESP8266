#ifndef MICRO_SD_H
#define MICRO_SD_H

#include <commonLibs.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>

#define D8 15  // GPIO15 on ESP8266
#define SD_CS_PIN D8  // Chip Select pin for the SD card module

// Function prototypes
class micro_sd {
  public:
    micro_sd();
    static void init(void);
    static StaticJsonDocument<2048> readData(void);
    static void writeData(void);
    static void writeData(String name_location = "day", char date_time[] = "00:00", float weight);
    static String loadDataFromSD(void);
};
#endif // MICRO_SD_H