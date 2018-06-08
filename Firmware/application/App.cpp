#include "App.h"

#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "lib/HUART.h"
#include "lib/HIIC.h"

#include "device/LED.h"

#include "application/Terminal.h"
#include "application/RTC.h"

#define MODE_BOOTING 0
#define MODE_WAITING_CONSOLE 1
#define MODE_WAITING_NTP 2
#define MODE_SET_DATE 3

volatile char App::terminal_current_buffer[] = {0x00};

App app = App();

App::App()
{
    this->init();
    this->onBoot();
}

void App::loop()
{
    while (1)
    {
        this->run();
    }
}

void App::init()
{
    this->working_mode = MODE_BOOTING;
    uart.init(115200);
    iic.init();
}

void App::onBoot()
{
    led.on();
    _delay_ms(2000);
    led.off();
    _delay_ms(1000);
    Time time;
    rtc.getTime(time);
    if (!rtc.validateTime(time))
    {
        for (uint8_t i = 0; i < 20; i++)
        {
            led.toggle();
            _delay_ms(100);
        }
    }
    led.off();
    this->working_mode = MODE_WAITING_CONSOLE;
    led.enableTimer(200);
}

void App::run()
{
    Time time;
    switch (this->working_mode)
    {
    case MODE_WAITING_CONSOLE:
        if (strcmp((const char *)App::terminal_current_buffer, "activate this console.") == 0)
        {
            led.disableTimer();
            led.on();
            _delay_ms(10000);
            terminal.send("");
            _delay_ms(2000);
            rtc.getTime(time);
            if (!rtc.validateTime(time))
            {
                this->working_mode = MODE_WAITING_NTP;
                led.enableTimer(800);
                terminal.send("sleep 5 && ntpd -n -q -p 1.cn.pool.ntp.org 2>/dev/null && date -u '+\%y.\%m.\%d-\%H:\%M:\%S' && sleep 5");
                _delay_ms(2000);
            }
            else
            {
                this->working_mode = MODE_SET_DATE;
            }
            this->flushTerminal();
        }
        break;

    case MODE_WAITING_NTP:
        if (!rtc.setTimeByString((char *)App::terminal_current_buffer))
        {
            if (strlen((char *)App::terminal_current_buffer) == 19)
            {
                terminal.send("sleep 5 && ntpd -n -q -p 1.cn.pool.ntp.org 2>/dev/null && date -u '+\%y.\%m.\%d-\%H:\%M:\%S' && sleep 5");
                _delay_ms(2000);
                this->flushTerminal();
            }
        }
        else
        {
            led.disableTimer();
            led.on();
            _delay_ms(10000);
            this->working_mode = MODE_WAITING_CONSOLE;
            led.enableTimer(200);
            terminal.flush();
        }
        break;

    case MODE_SET_DATE:
        led.enableTimer(50);
        _delay_ms(1000);
        led.disableTimer();
        led.off();
        _delay_ms(1000);
        char time_string[32];
        char command[64];
        rtc.getTimeString(time_string);
        sprintf(command, "%s '%s' ", "date -u -s ", time_string);
        terminal.send(command);
        led.disableTimer();
        led.on();
        _delay_ms(10000);
        this->working_mode = MODE_WAITING_CONSOLE;
        led.enableTimer(200);
        break;
    }
}

void App::flushTerminal()
{
    terminal.flush();
    memset((void *)App::terminal_current_buffer, 0x00, sizeof(App::terminal_current_buffer));
}

void App::onTerminalInterrupt()
{
    switch (this->working_mode)
    {
    case MODE_WAITING_CONSOLE:
        memcpy((void *)App::terminal_current_buffer, (const void *)(Terminal::buffer + (Terminal_BUFFER_SIZE - 22)), 22);
        break;

    case MODE_WAITING_NTP:
        memcpy((void *)App::terminal_current_buffer, (const void *)(Terminal::buffer + (Terminal_BUFFER_SIZE - 19)), 19);
        break;
    }
}
