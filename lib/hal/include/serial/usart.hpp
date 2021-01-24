/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "port/pin.hpp"
#include "serial/sercom_base.hpp"

namespace SAMD
{

struct UsartConfiguration
{
    enum class Mode
    {
        eExternalClk =
            static_cast<unsigned>(SercomBase::Mode::eSercomUsartExternalClk),
        eInternalClk =
            static_cast<unsigned>(SercomBase::Mode::eSercomUsartInternalClk)
    };

    enum class CommMode
    {
        eAsync = 0,
        eSync  = 1
    };

    enum class DataOrder
    {
        eMsbFirst = 0,
        eLsbFirst = 1,
    };

    enum class StopBitMode
    {
        eOneStopBit  = 0,
        eTwoStopBits = 1
    };

    enum class CharacterSize
    {
        e5Bits = 5,
        e6Bits = 6,
        e7Bits = 7,
        e8Bits = 0,
        e9Bits = 1
    };

    // Add Configuration options here
    static constexpr Mode Mode                   = Mode::eExternalClk;
    static constexpr CommMode CommMode           = CommMode::eAsync;
    static constexpr DataOrder DataOrder         = DataOrder::eMsbFirst;
    static constexpr StopBitMode StopBitMode     = StopBitMode::eOneStopBit;
    static constexpr CharacterSize CharacterSize = CharacterSize::e8Bits;

    // TODO: There are a whole lot more configurations that I have left out now.
    // The goal is to get a minimum viable serial port working before adding
    // full feature support.
};

template<typename CONFIG = UsartConfiguration>
class Usart : public SercomBase
{
public:
    enum class RxPad : uint8_t
    {
        eRxPad0 = 0,
        eRxPad1 = 1,
        eRxPad2 = 2,
        eRxPad3 = 3,

        eInvalid = 255
    };

    enum class TxPad : uint8_t
    {
        eTxdPad0    = 0,
        eTxdPad2    = 1,
        eTxdPad0Alt = 2,
        eXckPad1    = 0,
        eXckPad3    = 1,
        eRtsPad2    = 2,
        eCtsPad3    = 2,

        eInvalid = 255
    };

public:
    // I like the idea of compile time configuration here,
    // so really, this could support RAII. Init will be called from constructor.
    Usart(sercom_id_t id, ClockBase& sourceClock, Pin& objTxPin, Pin& objRxPin);
    ~Usart() override;

    void WriteSync(uint8_t data);
    void WriteAsync(const uint8_t* pData, unsigned dataLength);
    uint8_t ReadSync();

private:
    void Init();
    void Enable();
    void Disable();

    void ConfigurePins(Pin& objTxPin, Pin& objRxPin);

private:
    TxPad m_eTxPad;
    RxPad m_eRxPad;
};

} // namespace SAMD
