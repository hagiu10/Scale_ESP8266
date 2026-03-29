#ifndef HX711_IC_H
#define HX711_IC_H

#include <commonLibs.h>
#include <HX711.h>
#include <ArduinoJson.h>

#define D0 16 // GPIO16 on ESP8266
#define D3 0  // GPIO0 on ESP8266

#define HX_DT D0 
#define HX_SCK D3
#define FACTOR 21100.f // Calibration factor (adjust as needed)
#define GAIN 128 // Gain factor (128 for channel A, 64 for channel A, 32 for channel B)
#define TARE_ON_STARTUP 651631 // Optional: Set this to a known tare value if you want to tare the scale on startup (adjust as needed)

extern HX711 scale;

// Function prototypes
class hx711_ic {
  private:
    // You can add private members or functions here if needed
    static void findFactor(); // Optional: Function to find the calibration factor (FACTOR)
  public:
    hx711_ic();
    static void init(void);
    static float readWeight(void);
};
#endif // HX711_IC_H