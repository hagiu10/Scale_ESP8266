#include <hx711_ic.h>

HX711 scale;

/** Constructor
 */
hx711_ic::hx711_ic() {
 // Constructor implementation
}

/** Initialize the hx711
 */
void hx711_ic::init(void) {
    scale.begin(HX_DT, HX_SCK);
    scale.set_scale(2280.f); // Set the scale factor (adjust as needed)
    delay(500); // Allow some time for the HX711 to stabilize
    if (scale.is_ready()) {
        #ifdef DEBUG
        Serial.println("hx711_ic::init - HX711 is ready.");
        #endif
    } else {
        #ifdef DEBUG
        Serial.println("hx711_ic::init - HX711 not found.");
        #endif
        return;
    }

    scale.tare();  // Reset the scale to 0

    #ifdef DEBUG
    Serial.println("hx711_ic::init - HX711 initialized");
    #endif
}
/** Read weight from hx711
 */
float hx711_ic::readWeight(void) {
    if (!scale.is_ready()) {
        #ifdef DEBUG
        Serial.println("hx711_ic::readWeight - HX711 not ready");
        Serial.println("hx711_ic::readWeight - Trying to reinitialize...");
        #endif
        init(); // Try to reinitialize
        return 0.0;
    }
    float weight = scale.get_units(10); // Read average of 10 readings
    
    #ifdef DEBUG
    Serial.printf("hx711_ic::readWeight - Weight: %f\n", weight);
    #endif

    return weight;
}

