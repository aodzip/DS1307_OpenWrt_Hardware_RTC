#include "RTC.h"
#include <stdio.h>

RTC rtc = RTC();

void RTC::getTimeString(char *string)
{
    Time time;
    DS1307::getTime(time);
    sprintf(string, "20%02x.%02x.%02x-%02x:%02x:%02x", time.year, time.month, time.date, time.hour, time.min, time.sec);
}

bool RTC::setTimeByString(char *string)
{
    uint16_t year, month, date, hour, min, sec;
    uint8_t rt = sscanf(string, "%2x.%2x.%2x-%2x:%2x:%2x", &year, &month, &date, &hour, &min, &sec);
    if (rt == 6)
    {
        Time time;
        time.hour = hour;
        time.min = min;
        time.sec = sec;
        time.date = date;
        time.month = month;
        time.year = year;
        if (validateTime(time))
        {
            DS1307::setTime(time);
            return true;
        }
    }
    return false;
}

bool RTC::validateTime(Time &time)
{
    if (time.year < 0x17)
    {
        return false;
    }
    if (time.month < 0x01 || time.month > 0x12)
    {
        return false;
    }
    if (time.date < 0x01 || time.date > 0x31)
    {
        return false;
    }
    if (time.hour < 0x00 || time.hour > 0x23)
    {
        return false;
    }
    if (time.min < 0x00 || time.min > 0x59)
    {
        return false;
    }
    if (time.sec < 0x00 || time.sec > 0x59)
    {
        return false;
    }
}
