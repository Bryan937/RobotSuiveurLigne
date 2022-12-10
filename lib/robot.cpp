#include <avr/io.h>
#include <avr/interrupt.h>
#include "robot.h"

void Robot::disableBoutonInterrupts()
{
    interruptD2_.disableInterrupt();
    interruptD3_.disableInterrupt();
}

void Robot::shutDownRobot()
{
    disableBoutonInterrupts();
    timer_.stopTimer();
    led.ledOff();
    motor.stop();
    sound.clearNote();
}