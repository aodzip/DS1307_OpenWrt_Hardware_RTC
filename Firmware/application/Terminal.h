#pragma once
#include <inttypes.h>

#define Terminal_BUFFER_SIZE 24

class Terminal
{
  public:
    volatile static char buffer[Terminal_BUFFER_SIZE];
    
  public:
    Terminal();
    static void send(const char *str);
    static void flush();
};

extern Terminal terminal;
