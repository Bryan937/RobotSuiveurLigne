#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "eltauro.h"
#include "debug.h"

// Trouvé en divisant la frequence du microcontroleur par le prescaler soit 8,000,000/1024 = 7813
const uint16_t ONE_SECOND_OF_TIMER_1 = 7813;
const uint16_t HALF_SECOND_OF_TIMER_1 = 3906;
// Const pour 1er partie
const uint16_t GO_TO_NEXT_POLE_TIME = ONE_SECOND_OF_TIMER_1 * 3;
const uint16_t IMPULSION_START = 500;
const uint8_t MAX_SPEED_DELAY = 200;
const uint8_t IMPULSION_DEBUG_DELAY = 20;
// Const pour 2e partie
const uint16_t ADJUST_PATH_1 = HALF_SECOND_OF_TIMER_1 * 13;
const uint16_t ADJUST_PATH_2 = ONE_SECOND_OF_TIMER_1 * 7;
const uint16_t REACH_KL_POLE = HALF_SECOND_OF_TIMER_1 * 3;
const uint16_t FINISH_2ND_LEFT_TURN = ONE_SECOND_OF_TIMER_1 * 2;
const uint8_t IMPULSION_PATH = 100;
const uint8_t IMPULSION_EXIT = 40;
const uint8_t MAX_LEFT_TURN = 2;
const uint8_t STEP_INIT = 0;
// Const pour 3e partie
const uint16_t REACH_ZIGZAG_TIME = HALF_SECOND_OF_TIMER_1 * 14;
const uint16_t TIME_BETWEEN_ZIGZAG = HALF_SECOND_OF_TIMER_1 * 3;
const uint16_t IMPULSION_START_ZIGZAG = 40;
const uint16_t IMPULSION_START_LAST_FOLLOW_LINE = 100;
const uint16_t ZIG_ZAG_RIGHT_TURN = 600;
const uint8_t LEFT_SPEED = 130;
const uint8_t RIGHT_SPEED = 200;
const uint16_t DELAY_ZIG_ZAG = 1000;
const uint16_t DELAY_AJUST_OUT_ZIG_ZAG = 1600;
const uint8_t MAX_SPEED_ZIG_ZAG = 150;
const uint8_t MIN_SPEED_ZIG_ZAG = 0;
// Const des adresses de la distance des poteaux enregistrés
const uint8_t NO_MORE_POLE = 0x00;
const uint8_t FIRST_DECISION_ADDRESS = 0x01;
const uint8_t SECOND_DECISION_ADDRESS = 0x02;
const uint8_t THIRD_DECISION_ADDRESS = 0x03;
// Const et variable volatile pour interruption
const uint8_t DEBOUNCE_DELAY = 30;
volatile bool wasInterruptButtonPressed = false;
volatile bool wasWhiteButtonPressed = false;
volatile bool isDecisionMade = false;
volatile bool isZigZagReach = false;
volatile bool isZigZagEnd = false;
volatile bool isPoleDone = false;
volatile bool isMiddle = false;
volatile bool is2ndLeftTurnDone = false;

// Fonctions utilitaires
void resetVolatileVariable()
{
    isDecisionMade = false;
    isZigZagReach = false;
    isZigZagEnd = false;
    isPoleDone = false;
    isMiddle = false;
    is2ndLeftTurnDone = false;
}

bool isInterruptButtonPresed()
{
    return PIND & (1 << PIND2);
}

bool isWhiteButtonPresed()
{
    return !(PIND & (1 << PIND3));
}

void Eltauro::disableBoutonInterrupts()
{
    interruptD2_.disableInterrupt();
    interruptD3_.disableInterrupt();
}

// Partie A parcours
void Eltauro::RobotMachineStatePart1()
{
    robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_START);
    sensors_.lineTracker.startingLine();
    
    while (true)
    {
        switch (currentRobotStatePart1_)
        {
            case RobotStatePart1::FOLLOW_LINE :
                robot_.drive.impulsion(WheelDirection::FORWARD, MAX_SPEED_DELAY);
                while (true)
                {
                    robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_DEBUG_DELAY);
                    tracking_.startLineTrackingPart1();

                    if (sensors_.lineTracker.allSensor())
                    {
                        break;
                    }

                    if (!isPoleDone) 
                    { 
                        if (sensors_.poleSensor.isPoleDetected())
                        {
                            break;
                        }
                    }
                }

                if (sensors_.lineTracker.allSensor()) 
                {
                    endPart1_ = true;
                    break;
                }

                robot_.motor.stop();

                currentRobotStatePart1_ = RobotStatePart1::DETECT_POLE;
                break;

            case RobotStatePart1::DETECT_POLE :
                timer_.stopTimer();
                isPoleDone = true;
                timer_.startTimer(GO_TO_NEXT_POLE_TIME);
                robot_.led.ledRed();
                sensors_.poleSensor.poleDetected();
                robot_.led.ledOff();
                currentRobotStatePart1_ = RobotStatePart1::FOLLOW_LINE;
                break;
        }

        if (endPart1_)
        {
            robotSequence_ = Sequence::END;
            break;
        }
    }
}

// Partie B parcours
void Eltauro::RobotMachineStatePart2()
{
    robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_START);
    robot_.drive.straightLine();

    timer_.startTimer(ONE_SECOND_OF_TIMER_1);
    while (!isDecisionMade)
    {
        tracking_.startLineTrackingPart1();
    }
    timer_.stopTimer();
    resetVolatileVariable();

    while (true)
    {
        tracking_.chooseTracking();
        
        if (decision_.getDecisionAddress() == SECOND_DECISION_ADDRESS)
        {
            timer_.startTimer(REACH_KL_POLE);
            while (!isMiddle)
            {
                robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_DEBUG_DELAY);
                tracking_.startLineTrackingPart1();
            }
            resetVolatileVariable();
            timer_.stopTimer();

            robot_.motor.stop();
            decision_.makeDecision(isLeavingIntersection_);
            decision_.decrementDecisionAddress();
        }

        if ((tracking_.getNLeftTurn() >= MAX_LEFT_TURN) && (step_ == STEP_INIT))
        {
            timer_.startTimer(FINISH_2ND_LEFT_TURN);
            while (!is2ndLeftTurnDone)
            {
                tracking_.startLineTrackingPart1();
            }
            timer_.stopTimer();
            resetVolatileVariable();
            step_++;
        }

        if ((sensors_.lineTracker.sensorActivated(Sensor::LEFT) || sensors_.lineTracker.sensorActivated(Sensor::RIGHT)) && (tracking_.getNLeftTurn() >= MAX_LEFT_TURN))
        {
            robot_.motor.stop();

            if (decision_.getDecisionAddress() != SECOND_DECISION_ADDRESS)
            {
                decision_.makeDecision(isLeavingIntersection_);
            }
            isLeavingIntersection_ = !isLeavingIntersection_;

            if (!isLeavingIntersection_)
            {
                decision_.decrementDecisionAddress();
            }

            if (isLeavingIntersection_)
            {
                if (decision_.getDecisionAddress() == THIRD_DECISION_ADDRESS)
                {
                    isDecisionMade = false;
                    timer_.startTimer(ADJUST_PATH_1);
                }
                else if (decision_.getDecisionAddress() == FIRST_DECISION_ADDRESS)
                {
                    isDecisionMade = false;
                    timer_.startTimer(ADJUST_PATH_2);
                }

                robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_PATH);
                while (!isDecisionMade)
                {
                    tracking_.startLineTrackingIntersection();
                }
                timer_.stopTimer();
                resetVolatileVariable();
            }

            if (decision_.getDecisionAddress() <= NO_MORE_POLE)
            {
                robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_EXIT);
                while (!tracking_.isEndOfPart2())
                {
                    tracking_.startLineTrackingPart22();
                }
                break;
            }
        }
    }

    robot_.shutDownRobot();
    robotSequence_ = Sequence::DEPART_S;
}

// Partie S parcours
void Eltauro::RobotMachineStatePart3()
{
    resetVolatileVariable();
    step_ = STEP_INIT;
    timer_.startTimer(REACH_ZIGZAG_TIME);
    robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_START);
    sensors_.lineTracker.startingLine();
    while (true)
    {
        tracking_.startLineTrackingPart1();
        if (isZigZagReach && (step_ == STEP_INIT))
        {
            robot_.motor.stop();
            step_++;
            isZigZagReach = false;
            timer_.stopTimer();
        }
        else if (step_ != STEP_INIT)
        {
            robot_.drive.turnRightForNDegree(ZIG_ZAG_RIGHT_TURN);
            robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_START_ZIGZAG);
            robot_.motor.setLeftSpeed(LEFT_SPEED);
            robot_.motor.setRightSpeed(RIGHT_SPEED);
            while (sensors_.lineTracker.allSensorDesactivated());
            robot_.motor.stop();
            while (!endZigzag_)
            {
                if (sensors_.lineTracker.sensorActivated(Sensor::LEFT) || sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_LEFT))
                {
                    isZigZagEnd = false;
                    timer_.startTimer(TIME_BETWEEN_ZIGZAG);
                    robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_START_ZIGZAG);
                    robot_.motor.setLeftSpeed(MAX_SPEED_ZIG_ZAG);
                    robot_.motor.setRightSpeed(MIN_SPEED_ZIG_ZAG);
                    while (!sensors_.lineTracker.sensorActivated(Sensor::RIGHT))
                    {
                        if (isZigZagEnd)
                        {
                            endZigzag_ = true;
                            break;
                        }
                    }
                    robot_.motor.stop();
                    timer_.stopTimer();
                }
                
                if (sensors_.lineTracker.sensorActivated(Sensor::RIGHT) || sensors_.lineTracker.sensorActivated(Sensor::MIDDLE_RIGHT))
                {
                    robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_START_ZIGZAG);
                    robot_.motor.setLeftSpeed(MIN_SPEED_ZIG_ZAG);
                    robot_.motor.setRightSpeed(MAX_SPEED_ZIG_ZAG);
                    while (!sensors_.lineTracker.sensorActivated(Sensor::LEFT));
                    robot_.motor.stop();
                }
            }

            robot_.drive.impulsion(WheelDirection::BACKWARD, IMPULSION_START_ZIGZAG);
            robot_.motor.setLeftWheelDirection(WheelDirection::BACKWARD);
            robot_.motor.setLeftSpeed(LEFT_SPEED);
            robot_.motor.setRightSpeed(MIN_SPEED_ZIG_ZAG);
            while (!sensors_.lineTracker.sensorActivated(Sensor::LEFT));

            robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_START_LAST_FOLLOW_LINE);
            robot_.drive.goForwardForNTime(DELAY_AJUST_OUT_ZIG_ZAG);
            while (!sensors_.lineTracker.sensorActivated(Sensor::MIDDLE))
            {
                robot_.drive.turn(Direction::RIGHT, LEFT_SPEED);
            }

            robot_.motor.setBothWheelDirection(WheelDirection::FORWARD);
            robot_.drive.impulsion(WheelDirection::FORWARD, IMPULSION_START_LAST_FOLLOW_LINE);
            while (endZigzag_)
            {   
                tracking_.startLineTrackingPart1();
                if (sensors_.lineTracker.allSensorDesactivated())
                {
                    robot_.drive.autoPark();
                    break;
                }
            }
            break;
        }
    }
    robotSequence_ = Sequence::END;
}

// Choix de parcours
void Eltauro::chooseRobotMode()
{
    switch (robotSequence_)
    {
        case Sequence::CHOIX_DEPART:
            while (!wasInterruptButtonPressed)
            {
                if (!wasWhiteButtonPressed && robotSequence_ == Sequence::CHOIX_DEPART)
                {
                    robot_.led.ledGreen();
                    robotSequence_  = Sequence::DEPART_A;
                    wasWhiteButtonPressed = false;
                }
                if (wasWhiteButtonPressed && robotSequence_ == Sequence::DEPART_A)
                {
                    robot_.led.ledRed();
                    robotSequence_  = Sequence::DEPART_B;
                    wasWhiteButtonPressed = false;
                }
                else if (wasWhiteButtonPressed && robotSequence_ == Sequence::DEPART_B)
                {
                    robot_.led.ledAmber();
                    robotSequence_ = Sequence::DEPART_S;
                    wasWhiteButtonPressed = false;
                }

                while (robotSequence_ == Sequence::DEPART_S) 
                {
                    robot_.led.ledAmber();

                    if (wasWhiteButtonPressed)
                    {
                        robot_.led.ledGreen();
                        robotSequence_ = Sequence::DEPART_A;
                        wasWhiteButtonPressed = false;
                    }
                    else if (wasInterruptButtonPressed)
                    {
                        break;
                    }
                }
            }
            disableBoutonInterrupts();
            break;

        case Sequence::DEPART_A:
            robot_.led.greenLedBlinking();
            RobotMachineStatePart1();
            break;

        case Sequence::DEPART_B:
            robot_.led.redLedBlinking();
            RobotMachineStatePart2();
            break;

        case Sequence::DEPART_S:
            if (!tracking_.isEndOfPart2())
            {
                robot_.led.amberLedBlinking();
            }
            RobotMachineStatePart3();
            break;

        case Sequence::END:
            robot_.shutDownRobot();
            end_ = true;
            break;
    }
}

// Execute le robot
void Eltauro::exec()
{
    interruptD2_.enableInterrupt();
    interruptD3_.enableInterrupt();
    while (!end_)
    {
        chooseRobotMode();
    }
} 

// Routine interruption ISR
ISR(INT0_vect)
{
    _delay_ms(DEBOUNCE_DELAY);
    wasInterruptButtonPressed = isInterruptButtonPresed();
    EIFR |= (1 << INTF0);
}

ISR(INT1_vect)
{
    _delay_ms(DEBOUNCE_DELAY);
    wasWhiteButtonPressed = isWhiteButtonPresed();
    EIFR |= (1 << INTF1);
}

ISR(TIMER1_COMPA_vect)
{
    isDecisionMade = true;
    isZigZagEnd = true;
    isPoleDone = false;
    isZigZagReach = true;
    isMiddle = true;
    is2ndLeftTurnDone = true;
}