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
    scale.begin(HX_DT, HX_SCK, GAIN);
    scale.set_scale(FACTOR); // Set the scale factor (adjust as needed)
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
    // Used just for debugging and calibration purposes. You can remove this call in production code.
    // findFactor(); // Optional: Call this function to find the calibration factor (FACTOR) if needed

}
/** Read weight from hx711
 */
float hx711_ic::readWeight(void) {
    if (!scale.is_ready()) {
        #ifdef DEBUG
        Serial.println("hx711_ic::readWeight - HX711 not ready");
        Serial.println("hx711_ic::readWeight - Trying to reinitialize...");
        #endif
        return 0.0; // Return 0 if the scale is not ready
    }
    float weight = scale.get_units(10); // Read average of 10 readings
    
    #ifdef DEBUG
    Serial.printf("hx711_ic::readWeight - Weight: %.3f\n", weight);
    #endif

    return weight;
}

void hx711_ic::findFactor() {
    // This function can be used to find the calibration factor (FACTOR) by comparing the raw readings with known weights.
    // You can place a known weight on the scale and adjust the FACTOR until the get_units() function returns the correct weight.
#ifdef DEBUG
    // scale.tare(); // Reset the scale without any weight
    
    Serial.println("Place a known weight on the scale and read the raw value.");
    for(int i = 5; i > 0; --i) {
        Serial.printf("Reading in %d seconds...\n", i);
        delay(1000);
    }

    long rawValue = scale.read_average(10); // Get average raw value

    Serial.printf("Raw value for known weight: %ld\n", rawValue);
    // Adjust FACTOR based on the raw value and the known weight
#endif
}


