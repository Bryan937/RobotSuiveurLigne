#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor.h"

Motor :: Motor(WheelDirection wheelDirection)
{
    cli();
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
    TCNT0 = 0;
    TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B = (1 << CS01);
    OCR0A = 0;
    OCR0B = 0;
    sei();

    setBothWheelDirection(wheelDirection);
}

void Motor::setBothSpeed(uint8_t speed)
{
    OCR0A = speed;
    OCR0B = speed;
}

void Motor::setRightSpeed(uint8_t rightSpeed)
{
    OCR0B = rightSpeed;
}

void Motor::setLeftSpeed(uint8_t leftSpeed)
{
    OCR0A = leftSpeed;
}

void Motor::setBothWheelDirection(WheelDirection wheelDirection)
{
    switch (wheelDirection)
    {
        case WheelDirection::FORWARD:
            PORTB &= ~(1 << PB2); 
            PORTB &= ~(1 << PB5);
            break;

        case WheelDirection::BACKWARD:
            PORTB |= (1 << PB2); 
            PORTB |= (1 << PB5);
            break;
    }
}

void Motor::setRightWheelDirection(WheelDirection rightWheelDirection)
{
    switch (rightWheelDirection)
    {
        case WheelDirection::FORWARD:
            PORTB &= ~(1 << PB2); 
            break;

        case WheelDirection::BACKWARD:
            PORTB |= (1 << PB2); 
            break;
    }
}

void Motor::setLeftWheelDirection(WheelDirection leftWheelDirection)
{
    switch (leftWheelDirection)
    {
        case WheelDirection::FORWARD:
            PORTB &= ~(1 << PB5);
            break;

        case WheelDirection::BACKWARD:
            PORTB |= (1 << PB5);
            break;
    }
}

void Motor::stop()
{
    OCR0A = 0;
    OCR0B = 0;
}
