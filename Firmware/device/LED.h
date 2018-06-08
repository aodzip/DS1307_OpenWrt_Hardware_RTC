#pragma once
#include <inttypes.h>
class LED
{
  private:
    volatile static uint16_t count;
    volatile static uint16_t interval;

  public:
    LED();
    static bool shouldTimerAction();
    static void enableTimer(uint16_t interval);
    static void disableTimer();
    static void on();
    static void off();
    static void toggle();
};
extern LED led;
