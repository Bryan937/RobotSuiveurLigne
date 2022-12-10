#ifndef TRACKINGLINE_H
#define TRACKINGLINE_H

#include "robot.h"
#include "sensors.h"

class TrackingLine
{
public:
    TrackingLine() = default;

    void startLineTrackingPart1();
    void startLineTrackingPart21();
    void startLineTrackingPart22();
    void startLineTrackingMiddle();
    void startLineTrackingIntersection();

    void chooseTracking();

    uint8_t getNLeftTurn();
    uint8_t getNRightTurn();

    bool isEndOfPart2();

private:
    Robot robot_;
    Sensors sensors_;
    bool endPart2_ = false;
    uint8_t nLeftTurn_ = 0;
    uint8_t nRightTurn_ = 0;
};
#endif