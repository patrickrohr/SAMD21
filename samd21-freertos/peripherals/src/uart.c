/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "uart.h"
#include "error.h"
#include "sercom_private.h"

void uart_init(Uart_t* self, unsigned uSercomId, enum UartClock eClock, enum UartRxPad eRxPad, enum UartTxPad eTxPad, unsigned uBaudRate)
{
    // Initialize Sercom Peripheral. Resets and sets mode
    enum SercomMode eSercomMode = eUsartExternalClk;
    if (eUartInternalClk == eClock)
    {
        eSercomMode = eUsartInternalClk;
    }
    sercom_init(&self->objSercom, uSercomId, eSercomMode);

    
}

void uart_set_clock_mode(Uart_t* self, enum UartClock eClock);

void uart_set_comm_mode(Uart_t* self, enum UartCommMode eCommMode);

void uart_set_character_size(Uart_t* self);

void uart_set_data_order(Uart_t* self, enum UartDataOrder eDataOrder);

void uart_enable_parity_mode(Uart_t* self);

void uart_set_number_of_stop_bits(Uart_t* self, unsigned uNumberOfStopBits);
