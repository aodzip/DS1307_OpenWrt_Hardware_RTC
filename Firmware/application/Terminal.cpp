#include "Terminal.h"

#include <avr/interrupt.h>
#include <util/atomic.h>

#include <string.h>

#include "lib/HUART.h"

#include "application/App.h"

volatile char Terminal::buffer[] = {0x00};
char tmp[Terminal_BUFFER_SIZE] = {0x00};

Terminal terminal = Terminal();

Terminal::Terminal()
{
    sei();
}

void Terminal::send(const char *str)
{
    uart.putl0(str);
}

void Terminal::flush()
{
    memset((void *)Terminal::buffer, 0x00, Terminal_BUFFER_SIZE);
}

ISR(USART_RXC_vect)
{
    uint8_t ch;
    uart.read0(ch, true);
    memcpy(tmp, (void *)(Terminal::buffer + 1), Terminal_BUFFER_SIZE - 1);
    tmp[Terminal_BUFFER_SIZE - 1] = ch;
    memcpy((void *)Terminal::buffer, tmp, Terminal_BUFFER_SIZE);
    app.onTerminalInterrupt();
}
