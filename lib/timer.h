#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

class Timer
{
public:
    Timer() = default;
    
    void startTimer(uint16_t duration = UINT16_MAX);
    void setDuration(uint16_t duration);
    void stopTimer();
};
#endif
