#ifndef RTC_H
#define RTC_H

#include <commonLibs.h>
#include <RTClib.h>
#include <Wire.h>


#define D2 4  // GPIO4 on ESP8266
#define D1 5  // GPIO5 on ESP8266
#define SDA_PIN D2  // GPIO4 on ESP8266
#define SCL_PIN D1  // GPIO5 on ESP8266

class rtc {
  public:
    rtc();
    static void init(void);
    static String getDateTime(String partOfDateTime);
    static void setDateTime(int year, int month, int day, int hour, int minute, int second);
  private:
    static RTC_DS1307 _rtc;
};

#endif // RTC_H