#ifndef TransmitReceiveBoard
#define TransmitReceiveBoard
#include "mbed.h"

//#define MIN_BOARD

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
#define B1 D3
#define B2 D8
#define SERDEB D1
#define RCLK D15
#define SRCLK D14
#define LED1 D6
#define MOSI D11
#define MISO D12
#define SPICLK D13
#define DIO0 D9
#define NSS D10
#define BAUD 115200

#endif
#endif
