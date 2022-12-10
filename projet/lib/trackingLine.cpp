#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/io.h>
#include "debug.h"
#include "trackingLine.h"

// Const pour les vitesses d'impulsions
const uint16_t IMPULSION_TURN = 40;
const uint16_t IMPULSION_TRACKING = 120;
const uint16_t IMPULSION_LEFT_TURN = 20;
// Const pour le nombre de virages
const uint8_t MAX_TURN = 2;
// Const delai virage
const uint8_t LEFT_TURN_DELAY = 200;
const uint16_t DELAY_ADJUST_TURN = 850;
// Const pour les vitesses d'ajustements
const uint8_t MAX_SPEED = 200;
const uint8_t MEDIUM_SPEED = 150;
const uint8_t MOTOR_SPEED_TURN = 130;
const uint8_t IMPULSION_PATH = 50;

void TrackingLine::startLineTrackingPart1()
{
    if (sensors_.lineTracker.allSensor())
    {
        robot_.motor.stop();
    }
    else if (sensors_.lineTracker.onlyMiddleSensor())
    {
        robot_.drive.straightLine();
    }
    else if (sensors_.lineTracker.needAdjustmentRight())
    {
        sensors_.lineTracker.adjustRight();
    }
    else if (sensors_.lineTracker.needAdjustmentLeft())
    {
        sensors_.lineTracker.adjustLeft();
    }
    else
    {
        robot_.motor.stop();
    }
}

void TrackingLine::startLineTrackingMiddle()
{
    if (sensors_.lineTracker.onlyMiddleSensor())
    {
        robot_.drive.straightLine();
    }
    else if (sensors_.lineTracker.allSensorDesactivated())
    {
        robot_.motor.stop();
    }
    else if (sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_LEFT))
    {
        sensors_.lineTracker.adjustLeft2();
    }
    else if (sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_RIGHT))
    {
        sensors_.lineTracker.adjustRight2();
    }
}

void TrackingLine::startLineTrackingPart21()
{
    if (sensors_.lineTracker.onlyMiddleSensor())
    {
        robot_.drive.straightLine();
    }
    else if (sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_LEFT))
    {
        sensors_.lineTracker.adjustLeft2();
    }
    else if (sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_RIGHT))
    {
        sensors_.lineTracker.adjustRight2();
    }
    if (sensors_.lineTracker.sensorActivated(Sensor::LEFT))
    {
        sensors_.lineTracker.adjustLeft2();
    }
    if (sensors_.lineTracker.sensorActivated(Sensor::RIGHT))
    {
        nLeftTurn_++;
        sensors_.lineTracker.leftTurn();
        _delay_ms(LEFT_TURN_DELAY);

        robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_LEFT_TURN);

        while (sensors_.lineTracker.sensorActivated(Sensor::LEFT) || sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_LEFT) || sensors_.lineTracker.allSensorDesactivated())
        {
            robot_.motor.setRightSpeed(MAX_SPEED);
            robot_.motor.setLeftWheelDirection(WheelDirection::BACKWARD);
            robot_.motor.setLeftSpeed(MEDIUM_SPEED);
        }

        robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_TRACKING);
        while (sensors_.lineTracker.sensorActivated(Sensor::LEFT) || sensors_.lineTracker.sensorActivated(Sensor::RIGHT))
        {
            startLineTrackingPart1();
        }

        robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
    }
}

void TrackingLine::startLineTrackingPart22()
{
    if (sensors_.lineTracker.onlyMiddleSensor())
    {
        robot_.drive.straightLine();
    }
    else if (sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_LEFT))
    {
        sensors_.lineTracker.adjustLeft2();
    }
    else if (sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_RIGHT))
    {
        sensors_.lineTracker.adjustRight2();
    }

    if (sensors_.lineTracker.allSensor() && (nRightTurn_ >= MAX_TURN))
    {
        robot_.motor.stop();
        endPart2_ = true;
    }

    if (sensors_.lineTracker.sensorActivated(Sensor::RIGHT))
    {
        sensors_.lineTracker.adjustRight2();
    }

    if (sensors_.lineTracker.sensorActivated(Sensor::LEFT) && (nRightTurn_ < MAX_TURN))
    {
        robot_.motor.stop();
        nRightTurn_++;
        sensors_.lineTracker.rightTurn();
        robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_TURN);

        if (sensors_.lineTracker.sensorActivated(Sensor::RIGHT) || sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_RIGHT) || sensors_.lineTracker.allSensorDesactivated())
        {
            robot_.motor.setLeftSpeed(MAX_SPEED);
            robot_.motor.setRightWheelDirection(WheelDirection::BACKWARD);
            robot_.motor.setRightSpeed(MEDIUM_SPEED);
        }

        robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_TRACKING);

        while (sensors_.lineTracker.sensorActivated(Sensor::LEFT) || sensors_.lineTracker.sensorActivated(Sensor::RIGHT))
        {
            startLineTrackingPart1();
        }

        robot_.motor.stop();
        robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
    }
}

void TrackingLine::startLineTrackingIntersection()
{
    if (sensors_.lineTracker.onlyMiddleSensor())
    {
        robot_.drive.straightLine();
    }
    else if (sensors_.lineTracker.allSensorDesactivated())
    {
        robot_.motor.stop();
    }
    else if (sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_LEFT))
    {
        sensors_.lineTracker.adjustLeft2();
    }
    else if (sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_RIGHT))
    {
        sensors_.lineTracker.adjustRight2();
    }
    
    if (sensors_.lineTracker.sensorActivated(Sensor::LEFT))
    {
        robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_TURN);
        sensors_.lineTracker.adjustLeft2();
    }
    
    if (sensors_.lineTracker.sensorActivated(Sensor::RIGHT)) 
    {
        robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_TURN);
        sensors_.lineTracker.adjustRight2();
    }
}

void TrackingLine::chooseTracking()
{
    if (nLeftTurn_ < MAX_TURN)
    {
        startLineTrackingPart21();
    }
    else
    {
        startLineTrackingMiddle();
    }
}

uint8_t TrackingLine::getNLeftTurn()
{
    return nLeftTurn_; 
}

uint8_t TrackingLine::getNRightTurn() 
{ 
    return nRightTurn_; 
}

bool TrackingLine::isEndOfPart2() 
{ 
    return endPart2_; 
}