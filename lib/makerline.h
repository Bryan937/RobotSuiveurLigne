#ifndef MAKERLINE_H
#define MAKERLINE_H

#include <avr/io.h> 
#include "robot.h"

enum Sensor
{
    LEFT = PA3,
    MIDDLE_LEFT = PA4,
    MIDDLE = PA5,
    MIDDLE_RIGHT = PA6,
    RIGHT = PA7
};

class Makerline
{
public:
    Makerline() = default;

    uint16_t sensorValue(Sensor whichOne);
    bool sensorActivated(Sensor sensor);

    bool needAdjustmentLeft();
    bool needAdjustmentRight();
    void adjustLeft();
    void adjustRight();
    void adjustLeft2();
    void adjustRight2();

    bool onlyMiddleSensor();
    bool allSensor();
    bool allSensorDesactivated();

    void startingLine();
    void leftTurn();
    void rightTurn();

private:
    Robot robot_;
};
#endif