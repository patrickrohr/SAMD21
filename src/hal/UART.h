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
#include <stdbool.h>

#define SERCOM(val) (val * 0x400 + 0x42000800)

typedef enum {UART0} UART_enum;
typedef enum {UART_INITIALIZED} UARTStatus_enum;

typedef struct _UART UART_t;

/* Hidden UART Handle */
/*
struct _UART {
	SercomUsart * port;
	Packet_t * packet_rx;
	Packet_t * packet_tx;
	void * packet_rx_ptr;
	uint16_t packet_rx_length;
	uint16_t pin_rx;
	uint16_t pin_tx;
	uint16_t number;
	uint32_t status;
};
*/

UART_t * uart_get_instance(UART_enum uart_number);
bool uart_is_init(UART_t * self);
void uart_init(UART_t * self, uint16_t pin_rx, uint16_t pin_tx);
void uart_send(UART_t * self, char data);
void uart_send_packet(UART_t * self, Packet_t * packet);

// could become part of uart_create

#endif