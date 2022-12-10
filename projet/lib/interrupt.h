#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "interruptPin.h"

enum class InterruptMode
{
    RISING,
    FALLING,
    ANY
};

class Interrupt
{
public:
    Interrupt(InterruptPin pin);
    
    void enableInterrupt();
    void disableInterrupt();
    void interruptMode(InterruptMode mode);

private:
    InterruptPin pin_;
};
#endif