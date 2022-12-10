#ifndef DRIVE_H
#define DRIVE_H

#include <stdint.h>
#include "motor.h"
#include "sound.h"

enum class Direction 
{
    LEFT,
    RIGHT
};

class Drive
{
public:
    Drive() = default;

    void straightLine();
    void goForwardForNTime(const uint16_t forwardDelay);
    void goBackwardsForNTime(const uint16_t backwardDelay);
    void turnLeftForNDegree(const uint16_t degreeDelay); 
    void turnRightForNDegree(const uint16_t degreeDelay);
    void turn(Direction direction, uint8_t speed);

    void impulsion(WheelDirection direction, uint16_t delayChoosen);

    void autoPark();

private:
    Motor motor_;
    Sound sound_;
};
#endif