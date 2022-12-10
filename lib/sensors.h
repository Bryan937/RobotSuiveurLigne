#ifndef SENSORS_H
#define SENSORS_H

#include "makerline.h"
#include "distanceSensor.h"

class Sensors
{
public:
    Sensors() = default;
    
    Makerline lineTracker;
    DistanceSensor poleSensor;
};
#endif