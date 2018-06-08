#include "DS1307.h"

#include "lib/HIIC.h"
#include <util/delay.h>

#define DS1307_ADDRESS 0b1101000

void DS1307::setTime(Time &time)
{
    iic.beginTransmission(DS1307_ADDRESS);
    iic.write(0b00000000);
    iic.write(time.sec);
    iic.write(time.min);
    iic.write(time.hour);
    iic.write(time.weekDay);
    iic.write(time.date);
    iic.write(time.month);
    iic.write(time.year);
    iic.endTransmission(1);
    _delay_ms(10);
}

void DS1307::getTime(Time &time)
{
    iic.beginTransmission(DS1307_ADDRESS);
    iic.write(0b00000000);
    iic.endTransmission(0);
    iic.requestFrom(DS1307_ADDRESS, 7);
    time.sec = iic.read();
    time.min = iic.read();
    time.hour = iic.read();
    time.weekDay = iic.read();
    time.date = iic.read();
    time.month = iic.read();
    time.year = iic.read();
}
