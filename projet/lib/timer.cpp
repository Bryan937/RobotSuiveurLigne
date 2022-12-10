#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"


void Timer::startTimer(uint16_t duration) 
{
    cli();
    TCNT1 = 0;
    TCCR1A = 0;
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A) ;
    TCCR1C = 0;
    OCR1A = duration;
    sei();
}

void Timer::setDuration(uint16_t time)
{
    OCR1A = time;
}

void Timer::stopTimer()
{
    cli();
    TCCR1B = 0;
    sei();
}