/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "serial/usart.hpp"
#include "common/array.hpp"
#include "common/pair.hpp"
#include "common/vector.hpp"

namespace SAMD
{

template<typename CONFIG>
Usart<CONFIG>::Usart(
    sercom_id_t id,
    ClockSourceGeneric& sourceClock,
    Pin& objTxPin,
    Pin& objRxPin) :
    SercomBase(id, CONFIG::Mode, sourceClock),
    m_eTxPad(TxPad::eInvalid),
    m_eRxPad(RxPad::eInvalid)
{
    // Implements RAII
    ConfigurePins(objTxPin, objRxPin);
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
    samd_assert(m_eTxPad != TxPad::eInvalid, "TX Pad was not configured");
    samd_assert(m_eRxPad != RxPad::eInvalid, "RX Pad was not configured");

    // Port and Clock should be setup already
    // Configure mode
    auto reg_CTRLA = MakeRegisterGuard(&GetRegister()->Get().USART.CTRLA);
    auto reg_CTRLB = MakeRegisterGuard(&GetRegister()->Get().USART.CTRLB);

    RegisterGuard<SERCOM_USART_CTRLA_Type> tmp_CTRLA;
    RegisterGuard<SERCOM_USART_CTRLB_Type> tmp_CTRLB;

    tmp_CTRLA.Get().bit.MODE = static_cast<unsigned>(CONFIG::Mode);

    // TODO:
    tmp_CTRLA.Get().bit.CMODE = static_cast<unsigned>(CONFIG::CommMode);
    tmp_CTRLA.Get().bit.DORD  = static_cast<unsigned>(CONFIG::DataOrder);

    tmp_CTRLB.Get().bit.CHSIZE = static_cast<unsigned>(CONFIG::CharacterSize);

    // TODO: Parity mode setup
    // tmp_CTRLB.Get().PMODE = 0;
    // tmp_CTRLA.Get().bit.FORM

    tmp_CTRLB.Get().bit.SBMODE = static_cast<unsigned>(CONFIG::StopBitMode);

    // TODO:  When using internal clock, write BAUD register

    // Enable transmitter and receiver
    tmp_CTRLB.Get().bit.TXEN = 1;
    tmp_CTRLB.Get().bit.RXEN = 1;

    *reg_CTRLA = tmp_CTRLA;
    *reg_CTRLB = tmp_CTRLB;
}

template<typename CONFIG>
void Usart<CONFIG>::Enable()
{
    // TODO: Enable interrupts

    GetRegister()->Get().USART.CTRLA.bit.ENABLE = 1;
    while (GetRegister()->Get().USART.SYNCBUSY.bit.ENABLE) {}
}

template<typename CONFIG>
void Usart<CONFIG>::Disable()
{
    GetRegister()->Get().USART.CTRLA.bit.ENABLE = 0;
    while (GetRegister()->Get().USART.SYNCBUSY.bit.ENABLE) {}
}

template<typename CONFIG>
void Usart<CONFIG>::ConfigurePins(Pin& objTxPin, Pin& objRxPin)
{
    const auto& pinConfigs = GetPinConfigurations();

    SercomPinConfig* pTxPinConfig = nullptr;
    SercomPinConfig* pRxPinConfig = nullptr;

    for (const auto& pinConfig : pinConfigs)
    {
        if (pinConfig.SercomId == GetId())
        {

            if (pinConfig.PinId == objTxPin)
            {
                pTxPinConfig = &pinConfig;
            }

            if (pinConfig.PinId == objRxPin)
            {
                pRxPinConfig = &pinConfig;
            }
        }
    }

    samd_assert(pTxPinConfig != nullptr, "TX Pin Config not found");
    samd_assert(pRxPinConfig != nullptr, "RX Pin Config not found");

    objTxPin.Configure(Pin::Configuration::eOutputPull);
    objRxPin.Configure(Pin::Configuration::eInputPull);

    objTxPin.SetMultiplexingMode(pTxPinConfig->MultiplexingMode);
    objRxPin.SetMultiplexingMode(pRxPinConfig->MultiplexingMode);

    TxPad eTxPad = TxPad::eInvalid;
    RxPad eRxPad = RxPad::eInvalid;

    switch (pTxPinConfig->PadNumber)
    {
    case 0:
        eTxPad = TxPad::eTxdPad0;
    case 2:
        eTxPad = TxPad::eTxdPad2;
    case 1:
    case 3:
    default:
        samd_assert(false, "Wrong TX Pad Number!");
    }

    switch (pRxPinConfig->PadNumber)
    {
    case 0:
        eRxPad = RxPad::eRxPad0;
    case 1:
        eRxPad = RxPad::eRxPad1;
    case 2:
        eRxPad = RxPad::eRxPad2;
    case 3:
        eRxPad = RxPad::eRxPad3;
    default:
        samd_assert(false, "Wrong RX Pad Number!");
    }

    GetRegister()->Get().USART.CTRLA.bit.TXPO = static_cast<unsigned>(eTxPad);
    GetRegister()->Get().USART.CTRLA.bit.RXPO = static_cast<unsigned>(eRxPad);
}

template<typename CONFIG>
void Usart<CONFIG>::WriteSync(uint8_t data)
{
    while (!GetRegister()->Get().USART.INTFLAG.bit.DRE) {}
    GetRegister()->Get().USART.DATA.reg = data;
}

template<typename CONFIG>
void Usart<CONFIG>::WriteAsync(const uint8_t* pData, unsigned dataLength)
{
    // TODO: implement
}

template<typename CONFIG>
uint8_t Usart<CONFIG>::ReadSync()
{
    // TODO: Do this tomorrow...
    return 0;
}

// Some more TODO's:
// Remove the RegisterGuard template, it's really not needed.
// In order to support simulation, let's write a simuluation HAL.
// So the folder structure in the hal will look like:
// hal
//      include
//      src
//      sim
//
// Then let's just select sim during simulation builds, and src during Target
// builds. RegisterGuards were an interesting idea, but I don't think I can get
// around actually testing the HAL on real hardware. The benefit of this system
// though, is that once the code breaks on the hardware, I can work on breaking
// it in sim as well. A fix should then address both configurations
// simultaneously.

} // namespace SAMD
