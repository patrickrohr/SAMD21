/*
* UART.h
*
* Created: 6/12/2017 8:29:43 PM
*  Author: Patrick Rohr
*/ 

#ifndef __UART_H__
#define __UART_H__

#include "samd21.h"

//struct SERCOM;

//struct SERCOM * uart_create(struct SERCOM * uart_addr);
Sercom * uart_create();
void uart_send(Sercom * self, char data);

#endif