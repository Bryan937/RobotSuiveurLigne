#include <avr/io.h>
#include "uart.h"

Uart::Uart() 
{
    initializeUart();
}

void Uart::initializeUart()
{
    UBRR0H = 0;
    UBRR0L = BAUD_RATE;
    UCSR0A = 0;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

void Uart::transmit(uint8_t data) 
{
    while (!(UCSR0A & (_BV(UDRE0))));
    UDR0 = data;
}

uint8_t Uart::receive()
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}