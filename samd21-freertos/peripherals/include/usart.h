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
} Usart_t;

enum UsartClock
{
    eUsartInternalClk,
    eUsartExternalClk
};

enum UsartCommMode
{
    eUsartCommAsyc,
    eUsartCommSync
};

enum UsartRxPad
{
    eUsartRxPadPlaceHolder
};

enum UsartTxPad
{
    eUsartTxPadPlaceHolder
};

enum UsartDataOrder
{
    eUsartMsbFirst,
    eUsartLsbFirst
};

enum UsartCharacterSize
{
    placeholdercharactersize
};

void uart_init(Usart_t* self, unsigned uSercomId, enum UsartClock eClock, enum UsartCommMode eCommMode,
               enum UsartRxPad eRxPad, enum UsartTxPad eTxPad, unsigned uBaudRate);

void uart_set_character_size(Usart_t* self, enum UsartCharacterSize eCharSize);

void uart_set_data_order(Usart_t* self, enum UsartDataOrder eDataOrder);

void uart_set_number_of_stop_bits(Usart_t* self, unsigned uNumberOfStopBits);

// TODO
void uart_enable_parity_mode(Usart_t* self);
