#pragma once

#include <inttypes.h>

class Time
{
  public:
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t weekDay;
    uint8_t date;
    uint8_t month;
    uint8_t year;
};

class DS1307
{
  public:
    static void getTime(Time &time);
    static void setTime(Time &time);
};
