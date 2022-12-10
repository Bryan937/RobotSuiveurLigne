#ifndef ROBOT_H
#define ROBOT_H

#include <avr/io.h>
#include "motor.h"
#include "drive.h"
#include "led.h"
#include "button.h"
#include "memoire_24.h"
#include "sound.h"
#include "can.h"
#include "interrupt.h"
#include "timer.h"

class Robot
{
public:
    Robot() = default;

    Button interruptButton = Button(InterruptPin::D2);
    Button whiteButton = Button(InterruptPin::D3);
    Led led = Led(Port::B, PB0, PB1);
    Drive drive;
    Motor motor;
    Memoire24CXXX eeprom;
    Can can;
    Sound sound;

    void shutDownRobot();

private:
    Interrupt interruptD2_ = Interrupt(InterruptPin::D2);
    Interrupt interruptD3_ = Interrupt(InterruptPin::D3);
    Timer timer_;
    void disableBoutonInterrupts();
};
#endif