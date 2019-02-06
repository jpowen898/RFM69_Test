#ifndef MinTransmitBoard
#define MinTransmiteBoard
#include "mbed.h"
//Board specific pinouts
#ifdef MIN_BOARD

#define B1 NC
#define B2 NC
#define SERDEB P0_10
#define RCLK NC
#define SRCLK NC
#define LED1 P0_1
#define MOSI D11
#define MISO D12
#define SPICLK D13
#define DIO0 D9
#define NSS D10
#define BAUD 9600
#else

#endif
#endif