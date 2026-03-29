#include <rtc.h>

RTC_DS1307 rtc::_rtc;

rtc::rtc() {
  // Constructor
}

void rtc::init(void) {
    Wire.begin(SDA_PIN, SCL_PIN);
    delay(100);
    if(!_rtc.begin()){
        #ifdef DEBUG
        Serial.println("rtc::init - Couldn't find rtc");
        #endif
        return;
    }
    if (!_rtc.isrunning()) {
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        #ifdef DEBUG
        Serial.println("rtc::init - RTC was NOT running, set the time!");
        #endif
        return;
    }
    #ifdef DEBUG
    Serial.println("rtc::init - RTC initialized!");
    #endif
}

String rtc::getDateTime(String partOfDateTime = "second") {
    if (!_rtc.isrunning()) {
        #ifdef DEBUG
        Serial.println("rtc::getDateTime - RTC is NOT running!\nTrying to reinitialize...");
        #endif
        init(); // Try to initialize RTC again
        return String("");
    }
    DateTime now = _rtc.now();

    if (partOfDateTime.length() == 0) { 
        #ifdef DEBUG
        Serial.println("rtc::getDateTime - Invalid partOfDateTime parameter, returning empty string");
        #endif
        return String(""); 
    }

    partOfDateTime.toLowerCase();
    char firstChar = partOfDateTime.charAt(0);
    char secondChar = partOfDateTime.length() > 1 ? partOfDateTime.charAt(1) : '\0';

    switch(firstChar) {
        case 'y': // year
        return String(now.year(), DEC);
        case 'm': // month or minute
        if (secondChar == 'o') {
            return String(now.month(), DEC); // month as number
        }else{
            return String(now.minute(), DEC); // minute as number
        }
        case 'd': // day
        return String(now.day(), DEC);
        case 'h': // hour
        return String(now.hour(), DEC);
        case 's': // second
        default:
        return String(now.second(), DEC);
    }
}

void rtc::setDateTime(int year, int month, int day, int hour, int minute, int second) {
  _rtc.adjust(DateTime(year, month, day, hour, minute, second));
}
