#pragma once

#include "device/DS1307.h"

class RTC : public DS1307
{
  public:
    void getTimeString(char *string);
    bool setTimeByString(char *string);
    bool validateTime(Time &time);
};

extern RTC rtc;
