#pragma once

#include <avr/io.h>
#include <inttypes.h>

class HUART
{
  public:
    void init(uint32_t uart_0_baud);
    void write0(const uint8_t &c);
    bool read0(uint8_t &c, const bool &async = false);
    void flush0();
    void puts0(const char *s);
    void putl0(const char *s);
};

extern HUART uart;

#ifdef DEBUG
#include <stdio.h>

#define debug(format, args...)        \
    {                                 \
        char str[128];                \
        sprintf(str, format, ##args); \
        uart.putl0(str);              \
    }
#else
#define debug(format, args...)
#endif