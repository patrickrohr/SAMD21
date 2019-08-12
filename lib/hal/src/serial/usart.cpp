/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "serial/usart.hpp"

namespace SAMD
{

template<typename CONFIG>
Usart<CONFIG>::Usart(sercom_id_t id, ClockSourceGeneric& sourceClock) :
    SercomBase(id, CONFIG::Mode, sourceClock)
{
    // Implements RAII
    Init();
    Enable();
}

template<typename CONFIG>
Usart<CONFIG>::~Usart()
{
    Disable();
}

template<typename CONFIG>
void Usart<CONFIG>::Init()
{
    // Port and Clock should be setup already
    // Configure mode
    volatile auto& reg = GetRegister()->Get().USART;
    reg.CTRLA.MODE = static_cast<unsigned>(CONFIG::Mode);

    // TODO:
    reg.CTRLA.CMODE = 0;
    reg.CTRLA.RXPO = 0;
    reg.CTRLA.TXPO = 0;
    reg.CTRLA.DORD = 0;

    reg.CTRLB.CHSIZE = 0;

    // TODO: Parity mode setup
    // reg.CTRLB.PMODE = 0;
    // reg.CTRLA.FORM

    reg.CTRLB.SBMODE = 0;

    // TODO:  When using internal clock, write BAUD register

    // Enable transmitter and receiver
    reg.CTRLB.TXEN = 1;
    reg.CTRLB.RXEN = 1;
}

template<typename CONFIG>
void Usart<CONFIG>::Enable()
{

}

template<typename CONFIG>
void Usart<CONFIG>::Disable()
{

}

} // namespace SAMD
