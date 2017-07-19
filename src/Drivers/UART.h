/*
* UART.h
*
* Created: 6/12/2017 8:29:43 PM
*  Author: Patrick Rohr
*/ 

#ifndef __UART_H__
#define __UART_H__

#include "BufferPool.h" // I don't know if I want to include this here. Is this good design? Right now, the Packet definition is in here so I have to...
#include "samd21.h"

/* for consistency only... */
typedef Sercom UART_t;

UART_t * uart_create(UART_t * serial_port, int pin_rx, int pin_tx);
void uart_send(UART_t * self, char data);

#endif