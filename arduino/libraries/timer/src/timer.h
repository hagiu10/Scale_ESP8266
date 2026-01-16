#ifndef TIMER_H
#define TIMER_H

#include <commonLibs.h>

#define NR_TIMERS      1
#define MAX_COUNT      8388607      // ESP8266 only has one usable timer1, max count is only 8,388,607.
#define TIM_FREQUENCY  100000.0f    // 1 / 10us = 100,000 Hz (float)

typedef void (*timer_callback)  ();

#define TIM_DIV1_CLOCK          (80000000UL)          // 80000000 / 1   = 80.0  MHz
#define TIM_DIV16_CLOCK         (5000000UL)           // 80000000 / 16  = 5.0   MHz
#define TIM_DIV256_CLOCK        (312500UL)            // 80000000 / 256 = 312.5 KHz

#define USING_TIM_DIV1            true
#define USING_TIM_DIV16           false
#define USING_TIM_DIV256          false

#if ( defined(USING_TIM_DIV1) && USING_TIM_DIV1 )
  #warning Using TIM_DIV1_CLOCK for shortest and most accurate timer
  #define TIM_CLOCK_FREQ        TIM_DIV1_CLOCK
  #define TIM_DIV               TIM_DIV1
#elif ( defined(USING_TIM_DIV16) && USING_TIM_DIV16 )
  #warning Using TIM_DIV16_CLOCK for medium time and medium accurate timer
  #define TIM_CLOCK_FREQ        TIM_DIV16_CLOCK
  #define TIM_DIV               TIM_DIV16
#elif ( defined(USING_TIM_DIV256) && USING_TIM_DIV256 )
  #warning Using TIM_DIV256_CLOCK for longest timer but least accurate
  #define TIM_CLOCK_FREQ        TIM_DIV256_CLOCK
  #define TIM_DIV               TIM_DIV256
#else
  #warning Default to using TIM_DIV256_CLOCK for longest timer but least accurate
  #define TIM_CLOCK_FREQ        TIM_DIV256_CLOCK
  #define TIM_DIV               TIM_DIV256
#endif

// Function prototypes
class timer {
  private:
    static timer_callback _callback;
    static timer* _getInstance(void);
  public:
    timer();
    static void init(void);
    static void interrupt(timer_callback callback);
};
#endif // TIMER_H