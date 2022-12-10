#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include <stdint.h> 
#include "robot.h"

class DistanceSensor
{
public:
    DistanceSensor() = default;
    
    uint8_t sensorValue();
    bool isPoleDetected();
    void poleDetected();

private:
    Robot robot_;
    const uint8_t N_POLES_ADDRESS_ = 0x00;
    uint8_t currentAddress_ = 0x01;
    uint8_t nPolesDetected_ = 0;
    void writePoleInMemory(const uint8_t poleClose, const uint8_t poleFar);
    bool isPoleHigherThanSensorValue(const uint8_t sensorValueChosen);
    bool isPoleClose();
};
#endif