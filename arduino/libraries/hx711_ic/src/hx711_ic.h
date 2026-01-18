#ifndef HX711_IC_H
#define HX711_IC_H

#include <commonLibs.h>
#include <HX711.h>
#include <ArduinoJson.h>

#define D1 4  // GPIO14 on ESP8266
#define D2 5  // GPIO12 on ESP8266
#define HX_DT D1 
#define HX_SCK D2

extern HX711 scale;

// Function prototypes
class hx711_ic {
  public:
    hx711_ic();
    static void init(void);
    static float readWeight(void);
};
#endif // HX711_IC_H