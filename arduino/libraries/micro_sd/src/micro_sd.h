#ifndef MICRO_SD_H
#define MICRO_SD_H

#include <commonLibs.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>

#define D8 15  // GPIO15 on ESP8266
#define SD_CS_PIN D8  // Chip Select pin for the SD card module

/* Pin definitions for SD card module 
GPIO2 (D4) - Chip Select (CS)
GPIO13 (D7) - Master Out Slave In (MOSI)
GPIO12 (D6) - Master In Slave Out (MISO)
GPIO14 (D5) - Serial Clock (SCK)
*/
/*MISO = GPIO12, MOSI = GPIO13, SCLK = GPIO14*/
/*
#define PIN_SPI_SS   (15)
#define PIN_SPI_MOSI (13)
#define PIN_SPI_MISO (12)
#define PIN_SPI_SCK  (14)*/
extern bool sdOK;

// Function prototypes
class micro_sd {
  public:
    micro_sd();
    static void init(void);
    static StaticJsonDocument<2048> readFile(String path);
    static void writeFile(StaticJsonDocument<2048>& doc, String path);
    static void writeData(float weight, String name_location, String date_time);
    static String loadDataFromSD(void);
};
#endif // MICRO_SD_H