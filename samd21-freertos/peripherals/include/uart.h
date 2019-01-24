/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "sercom.h"

typedef struct
{
    Sercom_t objSercom;
} Uart_t;

enum UartClock
{
    eUartInternalClk,
    eUartExternalClk
};

enum UartCommMode
{
    eUartCommSync,
    eUartCommAsyc
};

enum UartRxPad
{
    eUartRxPadPlaceHolder
};

enum UartTxPad
{
    eUartTxPadPlaceHolder
};

enum UartDataOrder
{
    eUartMsbFirst,
    eUartLsbFirst
};

void uart_init(Uart_t* self, unsigned uSercomId, enum UartClock eClock, enum UartRxPad eRxPad, enum UartTxPad eTxPad, unsigned uBaudRate);

void uart_set_comm_mode(Uart_t* self, enum UartCommMode eCommMode);

void uart_set_character_size(Uart_t* self);

void uart_set_data_order(Uart_t* self, enum UartDataOrder eDataOrder);

void uart_enable_parity_mode(Uart_t* self);

void uart_set_number_of_stop_bits(Uart_t* self, unsigned uNumberOfStopBits);
