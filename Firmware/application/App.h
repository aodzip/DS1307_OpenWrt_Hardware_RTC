#pragma once

#include <inttypes.h>

class App
{
  private:
    volatile uint8_t working_mode;

  private:
    volatile static char terminal_current_buffer[32];

  private:
    void init();
    void onBoot();
    void run();
    void flushTerminal();

  public:
    App();
    void loop();
    void onInterrupt();
    void setFlashMode(const uint8_t &mode);
    void onTerminalInterrupt();
};

extern App app;
