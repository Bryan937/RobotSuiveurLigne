#include <string.h>
#include "uart.h"

#define DEBUG
#ifdef DEBUG
// Transférer les données un caractère à la fois
#define DEBUG_PRINT(string); Uart rs232; for(uint8_t i = 0; i < strlen(string); i++){rs232.transmit(string[i]);}
#else
#define DEBUG_PRINT(string) do {} while(false);
#endif