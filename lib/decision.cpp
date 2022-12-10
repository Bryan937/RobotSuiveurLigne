#define F_CPU 8000000UL

#include <util/delay.h>
#include "decision.h"

// Const pour 2e partie
const uint16_t DELAY_ADJUST_PATH_ENTRY = 1200;
const uint16_t DELAY_ADJUST_PATH_EXIT = 1500;
const uint16_t GO_BACKWARD_ONE_HALF_INCH_DELAY = 1000;
// Const pour la postion des poteaux
const uint8_t POLE_CLOSE = 0x00;
const uint8_t POLE_FAR = 0x01;
const uint8_t MAX_POLES_DETECTED = 3;
// Const des adresses de la distance des poteaux enregistrer
const uint8_t N_POLES_ADDRESS = 0x00;
const uint8_t FIRST_DECISION_ADDRESS = 0x01;
const uint8_t SECOND_DECISION_ADDRESS = 0x02;
const uint8_t THIRD_DECISION_ADDRESS = 0x03;
// Const pour direction du robot
const uint16_t ROTATION_15_DEGREE_DELAY = 400;
const uint8_t MOTOR_SPEED = 125;
// Const utilitaire
const uint16_t ONE_SECOND_DELAY = 1000;

// Fonction utilitaire
void oneSecondePause()
{
    _delay_ms(ONE_SECOND_DELAY);
}

// Lit dans la memoire pour savoir où se situe les poteaux
void Decision::readDecisionAddress(const uint8_t addressChosen)
{
    robot_.motor.stop();
    robot_.eeprom.lecture(addressChosen, &poleDistance_);
}

// Choisir le bon chemin lors d'un embranchement
void Decision::chooseRightPath(const uint8_t addressPathChosen)
{
    readDecisionAddress(addressPathChosen);

    if (poleDistance_ == POLE_CLOSE) 
    {
        robot_.drive.straightLine();
        _delay_ms(DELAY_ADJUST_PATH_ENTRY);
        robot_.motor.stop();

        while (!sensors_.lineTracker.sensorActivated(Sensor::LEFT) || !sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_LEFT))
        {
            robot_.drive.turn(Direction::LEFT, MOTOR_SPEED);
        }
    } 
    else if (poleDistance_ == POLE_FAR) 
    {
        robot_.drive.straightLine();
        _delay_ms(DELAY_ADJUST_PATH_ENTRY);
        robot_.motor.stop();

        while (!sensors_.lineTracker.sensorActivated(Sensor::RIGHT) || !sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_RIGHT))
        {
            robot_.drive.turn(Direction::RIGHT, MOTOR_SPEED);
        }
    }

    robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
}

// Choisir la bonne action a faire entre le secteur L et K (2e decision)
void Decision::choseRightAction(const uint8_t nPolesAddress)
{
    readDecisionAddress(nPolesAddress);

    if (poleDistance_ < MAX_POLES_DETECTED) 
    {
        robot_.drive.goBackwardsForNTime(GO_BACKWARD_ONE_HALF_INCH_DELAY);
    }
    else 
    {
        robot_.eeprom.lecture(SECOND_DECISION_ADDRESS, &poleDistance_);

        if (poleDistance_ == POLE_CLOSE) 
        {
            robot_.drive.turnLeftForNDegree(ROTATION_15_DEGREE_DELAY);
            oneSecondePause();
            while (!sensors_.lineTracker.sensorActivated(Sensor::MIDDLE))
            {
                robot_.drive.turn(Direction::LEFT, MOTOR_SPEED);
            }
        }
        else if (poleDistance_ == POLE_FAR) 
        {
            robot_.drive.turnRightForNDegree(ROTATION_15_DEGREE_DELAY);
            oneSecondePause();
            while (!sensors_.lineTracker.sensorActivated(Sensor::MIDDLE))
            {
                robot_.drive.turn(Direction::RIGHT, MOTOR_SPEED);
            }
        }

        robot_.motor.stop();
        robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
    }
}

// Tourne selon le chemin qu'il a pris pour sortir de l'intersection
void Decision::leaveIntersection(uint8_t poleDistance_Address)
{
    readDecisionAddress(poleDistance_Address);

    if (poleDistance_ == POLE_CLOSE) 
    {
        robot_.drive.goForwardForNTime(DELAY_ADJUST_PATH_EXIT);
        while (!sensors_.lineTracker.sensorActivated(Sensor::LEFT))
        {
            robot_.drive.turn(Direction::LEFT, MOTOR_SPEED);
        }
    }
    else if (poleDistance_ == POLE_FAR) 
    {
        robot_.drive.goForwardForNTime(DELAY_ADJUST_PATH_EXIT);
        while (!sensors_.lineTracker.sensorActivated(Sensor::RIGHT))
        {
            robot_.drive.turn(Direction::RIGHT, MOTOR_SPEED);
        }
    }
}

// Prendre une decision dependement de la postion du robot_ dans le parcours
void Decision::makeDecision(bool isLeavingIntersection)
{
    if (decisionAddress_ == FIRST_DECISION_ADDRESS && !isLeavingIntersection) 
    {
        robot_.led.ledRed();
        chooseRightPath(FIRST_DECISION_ADDRESS);
        robot_.led.ledOff();
    }
    else if (decisionAddress_ == FIRST_DECISION_ADDRESS && isLeavingIntersection) 
    {
        robot_.led.ledRed();
        leaveIntersection(FIRST_DECISION_ADDRESS);
        robot_.led.ledOff();
    }
    else if (decisionAddress_ == SECOND_DECISION_ADDRESS) 
    {
        choseRightAction(N_POLES_ADDRESS);
    }
    else if (decisionAddress_ == THIRD_DECISION_ADDRESS && !isLeavingIntersection) 
    {
        robot_.led.ledGreen();
        chooseRightPath(THIRD_DECISION_ADDRESS);
        robot_.led.ledOff();
    }
    else if (decisionAddress_ == THIRD_DECISION_ADDRESS && isLeavingIntersection) 
    {
        robot_.led.ledGreen();
        leaveIntersection(THIRD_DECISION_ADDRESS);
        robot_.led.ledOff();
    }
}

uint8_t Decision::getDecisionAddress()
{
    return decisionAddress_;
}

void Decision::decrementDecisionAddress()
{
    decisionAddress_--;
}