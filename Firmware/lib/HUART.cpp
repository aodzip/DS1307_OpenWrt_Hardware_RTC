#include "HUART.h"

HUART uart = HUART();

void HUART::init(uint32_t uart_0_baud = false)
{
    uart_0_baud = ((F_CPU) / (uart_0_baud * 16UL) - 1);
    UBRRL = uart_0_baud;
    UBRRH = (uart_0_baud >> 8);
    UCSRB |= _BV(RXEN) | _BV(TXEN) | _BV(RXCIE);
    UCSRC |= _BV(URSEL) | _BV(UCSZ0) | _BV(UCSZ1);
}

void HUART::write0(const uint8_t &c)
{
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
}

bool HUART::read0(uint8_t &c, const bool &async)
{
    if (async)
    {
        if (UCSRA & _BV(RXC))
        {
            c = UDR;
            return true;
        }
    }
    else
    {
        loop_until_bit_is_set(UCSRA, RXC);
        c = UDR;
        return true;
    }
    return false;
}

void HUART::flush0()
{
    while (UCSRA & _BV(RXC))
    {
        if (UDR)
            ;
    }
}

void HUART::puts0(const char *s)
{
    while (*s)
    {
        HUART::write0((uint8_t)*s++);
    }
}

void HUART::putl0(const char *s)
{
    HUART::puts0(s);
    HUART::puts0("\r\n");
}
