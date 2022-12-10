#ifndef BUTTON_H
#define BUTTON_H

#include "interruptPin.h"

class Button
{
public:
    Button(InterruptPin pin);
    bool isPressed();
    
private:
    InterruptPin pin_;
};
#endif