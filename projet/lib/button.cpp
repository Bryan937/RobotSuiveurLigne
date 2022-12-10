#define F_CPU 8000000UL

#include <avr/io.h>
#include "button.h"

Button::Button(InterruptPin pin) : pin_(pin) 
{
    DDRD &= ~(1 << PD2) & ~(1 << PD3);
}

bool Button::isPressed()
{
    switch (pin_)
    {
        case InterruptPin::D2:
            return PIND & (1 << PIND2);
        
        case InterruptPin::D3:
            return !(PIND & (1 << PIND3));
    }
    
    return false;
}