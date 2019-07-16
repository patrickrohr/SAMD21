/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "usart.h"
#include "error.h"
#include "sercom_private.h"

void usart_init(Usart_t* self, unsigned uSercomId, enum UsartClock eClock, enum UsartCommMode eCommMode,
                enum UsartRxPad eRxPad, enum UsartTxPad eTxPad, unsigned uBaudRate)
{
    // Initialize Sercom Peripheral. Resets and sets mode
    enum SercomMode eSercomMode = eSercomUsartExternalClk;

    if (eUsartInternalClk == eClock)
    {
        eSercomMode = eSercomUsartInternalClk;
    }
    sercom_init(&self->objSercom, uSercomId, eSercomMode);

    // TODO: Add the following default configuration to Config.in
    SERCOM_USART_CTRLA_Type objUsartCtrlA =
    {
        .bit.CMODE = eCommMode,  // 0 async
        .bit.RXPO  = eRxPad,
        .bit.TXPO  = eTxPad,
        .bit.DORD  = 0,  // 0 = MSB first
    };

    SERCOM_USART_CTRLB_Type objUsartCtrlB =
    {
        .bit.CHSIZE = 0, // 0 = 8 bits
        .bit.SBMODE = 0, // 0 = 1 stop bit
    };

    self->objSercom.pSercom->USART.CTRLA = objUsartCtrlA;
    self->objSercom.pSercom->USART.CTRLB = objUsartCtrlB;
}

void usart_set_character_size(Usart_t* self, enum UsartCharacterSize eCharSize)
{
    self->objSercom.pSercom->USART.CTRLB.bit.CHSIZE = eCharSize;
}

void usart_set_data_order(Usart_t* self, enum UsartDataOrder eDataOrder)
{
    self->objSercom.pSercom->USART.CTRLA.bit.DORD = eDataOrder;
}

void usart_enable_parity_mode(Usart_t* self);

void usart_set_number_of_stop_bits(Usart_t* self, unsigned uNumberOfStopBits)
{
    assert(uNumberOfStopBits > 0 && uNumberOfStopBits < 3);
    self->objSercom.pSercom->USART.CTRLB.bit.SBMODE = uNumberOfStopBits - 1;
}
