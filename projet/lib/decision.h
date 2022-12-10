#ifndef DECISION_H
#define DECISION_H

#include "robot.h"
#include "sensors.h"
#include "trackingLine.h"

class Decision
{
public:
    Decision() = default;

    void readDecisionAddress(const uint8_t addressChosen);
    void chooseRightPath(const uint8_t addressPathChosen);
    void choseRightAction(const uint8_t nPolesAddress);
    void leaveIntersection(uint8_t poleDistanceAddress);
    void makeDecision(bool isLeavingIntersection);

    uint8_t getDecisionAddress();
    void decrementDecisionAddress();

private:
    Robot robot_;
    Sensors sensors_;
    TrackingLine tracking_;
    uint8_t poleDistance_;
    uint8_t decisionAddress_ = 0x03;
};
#endif