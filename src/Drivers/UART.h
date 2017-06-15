/*
* UART.h
*
* Created: 6/12/2017 8:29:43 PM
*  Author: Patrick Rohr
*/ 

#ifndef __UART_H__
#define __UART_H__

struct SERCOM;

struct SERCOM * uart_create(struct SERCOM * uart_addr);
void uart_send(struct SERCOM * self, char data);

#endif