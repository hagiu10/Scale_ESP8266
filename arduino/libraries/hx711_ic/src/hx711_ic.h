#ifndef HX711_IC_H
#define HX711_IC_H

#include <commonLibs.h>
#include <HX711.h>
#include <ArduinoJson.h>

#define D0 16 // GPIO16 on ESP8266
#define D3 0  // GPIO0 on ESP8266

#define HX_DT D0 
#define HX_SCK D3

extern HX711 scale;

// Function prototypes
class hx711_ic {
  public:
    hx711_ic();
    static void init(void);
    static float readWeight(void);
};
#endif // HX711_IC_H