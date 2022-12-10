#ifndef ELTAURO_H
#define ELTAURO_H

#include "robot.h"
#include "sensors.h"
#include "interrupt.h"
#include "timer.h"
#include "eltauroStates.h"
#include "trackingLine.h"
#include "decision.h"

class Eltauro
{
public:
    Eltauro() = default;

    void exec();

private:
    Robot robot_;
    Sensors sensors_;
    TrackingLine tracking_;
    Timer timer_;
    Decision decision_;
    Interrupt interruptD2_ = Interrupt(InterruptPin::D2);
    Interrupt interruptD3_ = Interrupt(InterruptPin::D3);
    Sequence robotSequence_ = Sequence::CHOIX_DEPART;
    RobotStatePart1 currentRobotStatePart1_ = RobotStatePart1::FOLLOW_LINE;

    // Attribut utile pour la prise de decision
    bool end_ = false;
    bool endPart1_ = false;
    bool endZigzag_ = false;
    bool isLeavingIntersection_ = false;
    uint8_t step_ = 0;

    void disableBoutonInterrupts();

    // Fonction du comportement total du robot pour chaque partie du parcours
    void RobotMachineStatePart1();
    void RobotMachineStatePart2();
    void RobotMachineStatePart3();

    // Fonction pour choix de parcours
    void chooseRobotMode();
};
#endif