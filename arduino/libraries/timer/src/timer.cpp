#include <timer.h>

timer_callback timer::_callback = nullptr; // Initialize static member variable
/** Constructor
 */
timer::timer() {
 // Constructor implementation
}

/** Initialize the timer
 */
void timer::init(void) {
    double min_frequency = (double)TIM_CLOCK_FREQ / (double)MAX_COUNT;
    if(TIM_FREQUENCY < min_frequency) {
        #ifdef DEBUG
            Serial.printf("timer::init [error] Timer frequency %fHz is below minimum frequency %fHz. [%lu ms]\n", TIM_FREQUENCY, min_frequency, millis());
        #endif
        return;
    }
    uint32_t timerCount = (uint32_t) (TIM_CLOCK_FREQ / TIM_FREQUENCY);
    if (timerCount > MAX_COUNT) {
        #ifdef DEBUG
            Serial.printf("timer::init [error] Timer count %d exceeds maximum count of %d. [%lu ms]\n", timerCount, MAX_COUNT, millis());
        #endif
        return;
    }
    // Write the timer count to the timer
    timer1_write(timerCount);
    // Set up the timer interrupt and enable the timer
    timer1_enable(TIM_DIV, TIM_EDGE, TIM_LOOP);
#ifdef DEBUG
    Serial.printf("timer::init Timer has frequency %f Hz. [%lu ms]\n", TIM_FREQUENCY, millis());
    Serial.printf("timer::init Timer count set to %d. [%lu ms]\n", timerCount, millis());
#endif
}

/** Interrupt function
 * @param callback Callback function
 */
void timer::interrupt(timer_callback callback) {
    // Set the callback function
    if (callback == NULL) {
        #ifdef DEBUG
            Serial.printf("timer::interrupt [error] Callback function is NULL %p. [%lu ms]\n", callback, millis());
        #endif
        return;
    }
    timer::_callback = callback;
#ifdef DEBUG
    Serial.printf("timer::interrupt Callback function set to %p. [%lu ms]\n", timer::_callback, millis());
#endif
    // Attach the interrupt
    timer1_attachInterrupt(timer::_callback);
}
/** Get the instance of the timer
 */
timer* timer::_getInstance(void) {
    // Create a static instance of the timer class
    static timer timerInstance;
    // Return the instance
    return &timerInstance;
}