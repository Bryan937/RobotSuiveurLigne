#define F_CPU 8000000UL
#define ONE_MS 1

#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

// Const utilitaire
const uint8_t AMBER_DELAY = 1;
const uint8_t BLINKING_DELAY = 125;
const uint8_t BLINKING_TIMES = 8;
const uint8_t AMBER_TIME = 100;

Led::Led(Port port, uint8_t firstPinNumber, uint8_t secondPinNumber)
    :port_(port), firstPinNumber_(firstPinNumber), secondPinNumber_(secondPinNumber)
{
    initializePort();
}

void Led::initializePort()
{
    switch (port_)
    {
        case Port::A:
            DDRA |= (1 << firstPinNumber_) | (1 << secondPinNumber_);
            break;

        case Port::B:
            DDRB |= (1 << firstPinNumber_) | (1 << secondPinNumber_);
            break;

        case Port::C:
            DDRC |= (1 << firstPinNumber_) | (1 << secondPinNumber_);
            break;

        case Port::D:
            DDRD |= (1 << firstPinNumber_) | (1 << secondPinNumber_);
    }
}

void Led::ledRed()
{
    switch (port_)
    {
        case Port::A:
            PORTA |= (1 << firstPinNumber_);
            PORTA &= ~(1 << secondPinNumber_);
            break;

        case Port::B:
            PORTB |= (1 << firstPinNumber_);
            PORTB &= ~(1 << secondPinNumber_);
            break;

        case Port::C:
            PORTC |= (1 << firstPinNumber_);
            PORTC &= ~(1 << secondPinNumber_);
            break;

        case Port::D:
            PORTD |= (1 << firstPinNumber_);
            PORTD &= ~(1 << secondPinNumber_);
    }
}

void Led::ledGreen()
{
    switch (port_)
    {
        case Port::A:
            PORTA |= (1 << secondPinNumber_);
            PORTA &= ~(1 << firstPinNumber_);
            break;

        case Port::B:
            PORTB |= (1 << secondPinNumber_);
            PORTB &= ~(1 << firstPinNumber_);
            break;

        case Port::C:
            PORTC |= (1 << secondPinNumber_);
            PORTC &= ~(1 << firstPinNumber_);
            break;

        case Port::D:
            PORTD |= (1 << secondPinNumber_);
            PORTD &= ~(1 << firstPinNumber_);
    }
}

void Led::ledOff()
{
    switch (port_)
    {
        case Port::A:
            PORTA &= ~(1 << firstPinNumber_) & ~(1 << secondPinNumber_);
            break;

        case Port::B:
            PORTB &= ~(1 << firstPinNumber_) &~(1 << secondPinNumber_);
            break;

        case Port::C:
            PORTC &= ~(1 << firstPinNumber_) & ~(1 << secondPinNumber_);
            break;

        case Port::D:
            PORTD &= ~(1 << firstPinNumber_) & ~(1 << secondPinNumber_);
    }
}

void Led::ledAmber() 
{
    ledGreen();
    _delay_ms(AMBER_DELAY);
    ledRed();
    _delay_ms(AMBER_DELAY);
}

void Led::ledAmberTime(uint16_t timeMs)
{
    uint16_t nIteration = timeMs/2;
    for (uint16_t i = 0; i < nIteration; i++)
    {
        ledAmber();
    }

    ledOff();
}

void Led::redLedBlinking()
{
    for (int i = 0; i < BLINKING_TIMES; i++) 
    {
        ledRed();
        _delay_ms(BLINKING_DELAY);
        ledOff();
        _delay_ms(BLINKING_DELAY);
    }
}

void Led::greenLedBlinking()
{
    for (int i = 0; i < BLINKING_TIMES; i++) 
    {
        ledGreen();
        _delay_ms(BLINKING_DELAY);
        ledOff();
        _delay_ms(BLINKING_DELAY);
    }
}

void Led::amberLedBlinking()
{
    for (int i = 0; i < BLINKING_TIMES; i++) 
    {
        ledAmberTime(AMBER_TIME);
        _delay_ms(BLINKING_DELAY);
        ledOff();
        _delay_ms(BLINKING_DELAY);
    }
}