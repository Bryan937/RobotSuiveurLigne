#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "interrupt.h"

Interrupt::Interrupt(InterruptPin pin): pin_(pin)
{
    cli();
    DDRD &= ~(1 << PD2) & ~(1 << PD3);
    sei();
}

void Interrupt::enableInterrupt()
{
    switch (pin_)
    {
        case InterruptPin::D2:
            cli();
            EIMSK |= (1 << INT0);
            EICRA |= (1 << ISC01)| (1 << ISC00);
            sei();
            break;

        case InterruptPin::D3:
            cli();
            EIMSK |= (1 << INT1);
            EICRA |= (1 << ISC10);
            sei();
            break;
    }
}

void Interrupt::disableInterrupt()
{
    switch (pin_)
    {
        case InterruptPin::D2:
            cli();
            EIMSK &= ~(1 << INT0);
            sei();
            break;

        case InterruptPin::D3:
            cli();
            EIMSK &= ~(1 << INT1);
            sei();
            break;
    }   
}

void Interrupt::interruptMode(InterruptMode mode)
{
    switch (pin_)
    {
        case InterruptPin::D2:
            switch (mode)
            {
                case InterruptMode::RISING:
                    cli();
                    EICRA |= (1 << ISC01)| (1 << ISC00);
                    sei();
                    break;

                case InterruptMode::FALLING:
                    cli();
                    EICRA &= ~(1 << ISC00);
                    EICRA |= (1 << ISC01);
                    sei();
                    break;

                case InterruptMode::ANY:
                    cli();
                    EICRA |= (1 << ISC00);
                    EICRA &= ~(1 << ISC01);
                    sei();
                    break;
            }
            break;

        case InterruptPin::D3:
            switch (mode)
            {
                case InterruptMode::RISING:
                    cli();
                    EICRA |= (1 << ISC11)| (1 << ISC10);
                    sei();
                    break;

                case InterruptMode::FALLING:
                    cli();
                    EICRA &= ~(1 << ISC10);
                    EICRA |= (1 << ISC11);
                    sei();
                    break;

                case InterruptMode::ANY:
                    cli();
                    EICRA |= (1 << ISC10);
                    EICRA &= ~(1 << ISC11);
                    sei();
                    break;
            }
            break;
    }
}