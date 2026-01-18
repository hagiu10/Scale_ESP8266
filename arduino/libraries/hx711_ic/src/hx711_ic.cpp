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
    scale.tare();  // Reset the scale to 0
}
/** Read weight from hx711
 */
float hx711_ic::readWeight(void) {
    float weight = scale.get_units(10); // Read average of 10 readings
    return weight;
    #ifdef DEBUG
    Serial.printf("Weight: %f\n", weight);
    #endif
}

