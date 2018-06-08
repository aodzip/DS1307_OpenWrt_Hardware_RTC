#include "LED.h"

#include <avr/io.h>
#include <avr/interrupt.h>

LED led = LED();
volatile uint16_t LED::count = 0;
volatile uint16_t LED::interval = 0;

LED::LED()
{
    DDRD |= _BV(PD2);
    PORTD &= ~_BV(PD2);
}

bool LED::shouldTimerAction()
{
    if (LED::count >= LED::interval)
    {
        LED::count = 0;
        return true;
    }
    LED::count++;
}

void LED::enableTimer(uint16_t interval)
{
    interval = interval / 24;
    if (interval != LED::interval)
    {
        LED::interval = interval;
        TIMSK |= _BV(TOIE0);
        TCNT0 = 0x00;
        TCCR0 = 0b101;
    }
    sei();
}

void LED::disableTimer()
{
    TIMSK &= ~_BV(TOIE0);
    TCCR0 = 0x00;
    LED::count = 0;
}

void LED::on()
{
    PORTD |= _BV(PD2);
}

void LED::off()
{
    PORTD &= ~_BV(PD2);
}

void LED::toggle()
{
    PORTD ^= _BV(PD2);
}

ISR(TIMER0_OVF_vect)
{
    if (LED::shouldTimerAction())
    {
        LED::toggle();
    }
}
