#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

enum class WheelDirection
{
    FORWARD,
    BACKWARD
};

class Motor
{
public:
    Motor(WheelDirection direction = WheelDirection::FORWARD);

    void setBothSpeed(uint8_t speed);
    void setRightSpeed(uint8_t speedRight);
    void setLeftSpeed(uint8_t speedLeft);

    void setBothWheelDirection(WheelDirection wheelDirection);
    void setRightWheelDirection(WheelDirection rightWheelDirection);
    void setLeftWheelDirection(WheelDirection leftWheelDirection);

    void stop();
};
#endif