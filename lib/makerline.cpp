#define F_CPU 8000000UL

#include <util/delay.h>
#include "debug.h"
#include "makerline.h"
#include "led.h"

// Const utilitaire
const uint8_t ENABLE_VALUE = 150;
// Const pour ajustement du suivi de ligne
const uint8_t SPEED_ADJUSTEMENT = 120;
const uint8_t SPEED_ADJUSTEMENT_2 = 135;
// Const pour virages 90°
const uint8_t MOTOR_SPEED_TURN = 130;
const uint16_t DELAY_ADJUST_TURN = 850;

uint16_t Makerline::sensorValue(Sensor whichOne)
{
    switch(whichOne)
    {
        case Sensor::LEFT:
            return robot_.can.lecture(LEFT);

        case Sensor::MIDDLE_LEFT:
            return robot_.can.lecture(MIDDLE_LEFT);

        case Sensor::MIDDLE:
            return robot_.can.lecture(MIDDLE);

        case Sensor::MIDDLE_RIGHT:
            return robot_.can.lecture(MIDDLE_RIGHT);

        case Sensor::RIGHT:
            return robot_.can.lecture(RIGHT);
    }

    return 0;
}

bool Makerline::sensorActivated(Sensor sensor)
{
    return (sensorValue(sensor) > ENABLE_VALUE);
}

bool Makerline::needAdjustmentLeft()
{
    return ((sensorActivated(MIDDLE_LEFT) || sensorActivated(LEFT)) && !sensorActivated(MIDDLE_RIGHT) && !sensorActivated(RIGHT));
}

bool Makerline::needAdjustmentRight()
{
    return ((sensorActivated(MIDDLE_RIGHT) || sensorActivated(RIGHT)) && !sensorActivated(MIDDLE_LEFT) && !sensorActivated(LEFT));
}

bool Makerline::onlyMiddleSensor()
{
    return (sensorActivated(MIDDLE) && !sensorActivated(MIDDLE_LEFT) && !sensorActivated(MIDDLE_RIGHT) && !sensorActivated(LEFT) && !sensorActivated(RIGHT));
}

bool Makerline::allSensor()
{
    return (sensorActivated(LEFT) && sensorActivated(MIDDLE_LEFT) && sensorActivated(MIDDLE) && sensorActivated(MIDDLE_RIGHT) && sensorActivated(RIGHT));
}

bool Makerline::allSensorDesactivated()
{
    return (!sensorActivated(LEFT) && !sensorActivated(MIDDLE_LEFT) && !sensorActivated(MIDDLE) && !sensorActivated(MIDDLE_RIGHT) && !sensorActivated(RIGHT));
}

// Gérer les ajustement necessaires en cas de deviation pour la partie 1

void Makerline::adjustRight()
{
    robot_.motor.stop();

    while (!onlyMiddleSensor())
    {
        robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
        robot_.motor.setLeftSpeed(SPEED_ADJUSTEMENT);

        if (allSensor())
        {
            break;
        }
    }

    robot_.motor.stop();
}

void Makerline::adjustLeft()
{
    robot_.motor.stop();

    while (!onlyMiddleSensor())
    {
        robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
        robot_.motor.setRightSpeed(SPEED_ADJUSTEMENT);

        if (allSensor())
        {
            break;
        }
    }

    robot_.motor.stop();
}

// Gérer les ajustement necessaires en cas de deviation pour la partie 2

void Makerline::adjustRight2()
{
    robot_.motor.stop();

    while (!onlyMiddleSensor())
    {
        robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
        robot_.motor.setLeftSpeed(SPEED_ADJUSTEMENT_2);

        if (sensorActivated(Sensor::LEFT) || sensorActivated(Sensor::RIGHT) || allSensor())
        {
            break;
        }
    }

    robot_.motor.stop();
}

void Makerline::adjustLeft2()
{
    robot_.motor.stop();

    while (!onlyMiddleSensor())
    {
        robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
        robot_.motor.setRightSpeed(SPEED_ADJUSTEMENT_2);

        if (sensorActivated(Sensor::LEFT) || sensorActivated(Sensor::RIGHT) || allSensor())
        {
            break;
        }
    }

    robot_.motor.stop();
}

void Makerline::startingLine()
{
    while (allSensor())
    {
        robot_.drive.straightLine();
    }
}

// Pour les virages à 90°

void Makerline::leftTurn()
{
    if (allSensorDesactivated())
    {
        robot_.drive.straightLine();
    }
    else 
    {
        while (!allSensorDesactivated())
        {
            robot_.drive.straightLine();
        }
    }

    _delay_ms(DELAY_ADJUST_TURN);

    while (!sensorActivated(Sensor::RIGHT))
    {
        robot_.drive.turn(Direction::RIGHT, MOTOR_SPEED_TURN);
    }

    robot_.motor.stop();
    robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
}

void Makerline::rightTurn()
{
    if (allSensorDesactivated())
    {
        robot_.drive.straightLine();
    }
    else 
    {
        while (!allSensorDesactivated())
        {
            robot_.drive.straightLine();
        }
    }

    _delay_ms(DELAY_ADJUST_TURN);

    while (!sensorActivated(Sensor::LEFT))
    {
        robot_.drive.turn(Direction::LEFT, MOTOR_SPEED_TURN);
    }

    robot_.motor.stop();
    robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
}