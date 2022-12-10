#ifndef LED_H
#define LED_H

#include <stdint.h>

enum class Port 
{
    A,
    B,
    C,
    D
};

class Led 
{
public:
    Led(Port port, uint8_t firstPinNumber, uint8_t secondPinNumber);

    void ledRed();
    void ledGreen();
    void ledOff();
    void ledAmber();
    void ledAmberTime(uint16_t timeMs);
    
    void redLedBlinking();
    void greenLedBlinking();
    void amberLedBlinking();

private:
    Port port_;
    uint8_t firstPinNumber_;
    uint8_t secondPinNumber_;
    void initializePort();
};
#endif