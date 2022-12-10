#ifndef UART_H
#define UART_H

#include <stdint.h>

class Uart 
{
public:
    Uart();

    void transmit(uint8_t data);
    uint8_t receive();

private:
    const uint8_t BAUD_RATE = 0xCF;
    void initializeUart();
};
#endif