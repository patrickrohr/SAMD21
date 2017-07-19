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

typedef enum {UART0} UARTn_enum;

typedef struct {
	SercomUsart * UART; // to be consistent with SAMD header definitions
	uint16_t pin_rx; // lower case to distinguish from SAMD header definitions
	uint16_t pin_tx;
	uint32_t uart_number;
} UART_t;

void uart_create(UART_t * self);
void uart_send(UART_t * self, char data);

// could become part of uart_create
UART_t * uart_populate_handle(UART_t * uart, UARTn_enum uart_number, uint16_t pin_rx, uint16_t pin_tx);

#endif