#define F_CPU 8000000UL
#define ONE_MS 1

#include <avr/io.h>
#include <util/delay.h>
#include "distanceSensor.h"

// Const pour valeurs du capteur
const uint8_t DISTANCE_SENSOR = PA0;
const uint8_t NO_POLE_SENSOR_VALUE = 16;
const uint8_t MIDDLE_DISTANCE_SENSOR_VALUE = 30; 
const uint8_t DEBOUNCE_SENSOR_DELAY = 50;
// Const utilitaire
const uint8_t WRITING_DELAY = 5;
const uint8_t BIT_SHIFT_OF_TWO = 2;
// Const pour l'écriture en mémoire des poteaux
const uint8_t MAX_NUMBER_OF_POLES = 3;
const uint8_t POLE_CLOSE = 0x00;
const uint8_t POLE_FAR = 0x01;

uint8_t DistanceSensor::sensorValue()
{
    return robot_.can.lecture(DISTANCE_SENSOR) >> BIT_SHIFT_OF_TWO;
}

bool DistanceSensor::isPoleHigherThanSensorValue(const uint8_t sensorValueChosen) 
{
    if (sensorValue() > sensorValueChosen) 
    {
        _delay_ms(DEBOUNCE_SENSOR_DELAY);
        return sensorValue() > sensorValueChosen;
    }

    return false;
}

bool DistanceSensor::isPoleDetected()
{
    if (nPolesDetected_ < MAX_NUMBER_OF_POLES) 
    {
        robot_.motor.stop();
        return isPoleHigherThanSensorValue(NO_POLE_SENSOR_VALUE);
    }

    return false;
}

bool DistanceSensor::isPoleClose() 
{
    return isPoleHigherThanSensorValue(MIDDLE_DISTANCE_SENSOR_VALUE);
}

void DistanceSensor::writePoleInMemory(const uint8_t poleClose, const uint8_t poleFar)
{
    nPolesDetected_++;
    robot_.eeprom.ecriture(N_POLES_ADDRESS_, nPolesDetected_);
    _delay_ms(WRITING_DELAY);

    if (isPoleClose()) 
    {
        robot_.eeprom.ecriture(currentAddress_, poleClose);
    }
    else 
    {
        robot_.eeprom.ecriture(currentAddress_, poleFar);
    }
    
    currentAddress_++;
}


void DistanceSensor::poleDetected()
{
    switch(nPolesDetected_)
    {
        case 0:
            writePoleInMemory(POLE_CLOSE, POLE_FAR);
            break;

        case 1:
            writePoleInMemory(POLE_CLOSE, POLE_FAR);
            break;

        case 2:
            writePoleInMemory(POLE_CLOSE, POLE_FAR);
            break;
    }

    if (isPoleClose()) 
    {
        robot_.sound.playHighNote();
    }
    else 
    {
        robot_.sound.playDeepNote();
    }
}