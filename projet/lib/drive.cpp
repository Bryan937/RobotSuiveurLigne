#define F_CPU 8000000UL
#define ONE_MS 1

#include <util/delay.h>
#include "drive.h"

// Const générale pour les manoeuvres
const uint8_t TURNING_SPEED = 150;
const uint8_t IMPULSION_SPEED = 200;
// Const pour la méthode autoPark 
const uint8_t BOTH_WHEEL_BACKWARD_SPEED = 180;
const uint8_t LEFT_WHEEL_FORWARD_SPEED = 130;
const uint8_t RIGHT_WHEEL_FORWARD_SPEED = 160;
const uint16_t BACKWARD_DELAY = 1200;
const uint16_t FORWARD_DELAY = 1800;
const uint16_t TURN_RIGHT_DELAY = 850;
const uint8_t IMPULSION_AUTO_PARK = 100;
const uint16_t DELAY_STRAIGHT_LINE = 2000;
// Const pour straight line
const uint8_t LEFT_WHEEL_SPEED = 120;
const uint8_t RIGHT_WHEEL_SPEED = 150;

// Fonction utilitaire
void delay(uint16_t delayChosen)
{
    for (uint16_t delay = 0; delay < delayChosen; delay++) 
    {
        _delay_ms(ONE_MS);
    }
}

void Drive::straightLine() 
{
    motor_.setBothWheelDirection(WheelDirection::FORWARD);
    motor_.setLeftSpeed(LEFT_WHEEL_SPEED);
    motor_.setRightSpeed(RIGHT_WHEEL_SPEED);
}

void Drive::goForwardForNTime(const uint16_t forwardDelay)
{
    motor_.setBothWheelDirection(WheelDirection::FORWARD);
    motor_.setLeftSpeed(LEFT_WHEEL_FORWARD_SPEED);
    motor_.setRightSpeed(RIGHT_WHEEL_FORWARD_SPEED);
    delay(forwardDelay);
    motor_.stop();
}

void Drive::goBackwardsForNTime(const uint16_t backwardDelay)
{
    motor_.setBothWheelDirection(WheelDirection::BACKWARD);
    motor_.setLeftSpeed(BOTH_WHEEL_BACKWARD_SPEED);
    motor_.setRightSpeed(BOTH_WHEEL_BACKWARD_SPEED);
    delay(backwardDelay);
    motor_.stop();
}

void Drive::turnLeftForNDegree(const uint16_t degreeDelay) 
{
    motor_.setLeftWheelDirection(WheelDirection::FORWARD);
    motor_.setLeftSpeed(TURNING_SPEED);
    motor_.setRightWheelDirection(WheelDirection::BACKWARD);
    motor_.setRightSpeed(TURNING_SPEED);
    delay(degreeDelay);
    motor_.stop();
    motor_.setBothWheelDirection(WheelDirection::FORWARD);    
}

void Drive::turnRightForNDegree(const uint16_t degreeDelay) 
{
    motor_.setLeftWheelDirection(WheelDirection::BACKWARD);
    motor_.setLeftSpeed(TURNING_SPEED);
    motor_.setRightWheelDirection(WheelDirection::FORWARD);
    motor_.setRightSpeed(TURNING_SPEED);
    delay(degreeDelay);
    motor_.stop();
    motor_.setBothWheelDirection(WheelDirection::FORWARD);
}

void Drive::turn(Direction direction, uint8_t speed)
{
    if (direction == Direction::LEFT)
    {
        motor_.setRightWheelDirection(WheelDirection::FORWARD);
        motor_.setLeftWheelDirection(WheelDirection::BACKWARD);
        motor_.setBothSpeed(speed);
    }
    else if (direction == Direction::RIGHT)
    {
        motor_.setLeftWheelDirection(WheelDirection::FORWARD);
        motor_.setRightWheelDirection(WheelDirection::BACKWARD);
        motor_.setBothSpeed(speed);
    }
}

void Drive::impulsion(WheelDirection direction, uint16_t delayChoosen)
{
    motor_.setBothWheelDirection(direction);
    motor_.setBothSpeed(IMPULSION_SPEED);
    delay(delayChoosen);
}

void Drive::autoPark()
{
    goBackwardsForNTime(BACKWARD_DELAY);
    turnRightForNDegree(TURN_RIGHT_DELAY);
    _delay_ms(DELAY_STRAIGHT_LINE);
    impulsion(WheelDirection::FORWARD, IMPULSION_AUTO_PARK);
    goForwardForNTime(FORWARD_DELAY);
    motor_.stop();
    sound_.playHighNote();
}