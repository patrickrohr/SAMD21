/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <common/id_traits.hpp>
#include <hal_utils/register.hpp>
#include <cstdint>
#include <samd21.h>

namespace SAMD
{

class Pin
{
public:
    using pin_id_t = id_traits<Pin, uint8_t>;

    enum class Port : uint8_t
    {
        ePortA = 0,
        ePortB = 1
    };

    enum class Direction
    {
        eInput  = 0,
        eOutput = 1
    };

    enum class Configuration
    {
        eInputStandard,
        eInputPull,
        eTotemPoleDisabledIn, // Totem pole with disabled input
        eTotemPoleEnabledIn,  // Totem pole with enabled input
        eOutputPull,
        eAnalog,         // Analog I/O
        eReset = eAnalog // Reset state
    };

    enum class MultiplexingMode
    {
        eModeA,
        eModeB,
        eModeC,
        eModeD,
        eModeE,
        eModeF,
        eModeG,
        eModeH,

        eEIC = eModeA,
        eREF = eModeB,
        eADC = eModeB,
        eAC = eModeB,
        ePTC = eModeB,
        eDAC = eModeB,
        eSercom = eModeC,
        eSercomAlt = eModeD,
        eTC = eModeE, // also TCC
        eTCC = eModeF,
        eCOM = eModeG,
        eGCLK = eModeH // also AC
    };

public:
    constexpr Pin(Port ePort, pin_id_t pinId) :
        m_uGlobalId(pinId.Get() + ((ePort == Port::ePortA) ? 32 : 0)),
        m_ePort(ePort),
        m_localId(pinId)
    {
    }

    // You shouldn't use this directly, but I'll leave it public for now.
    void Configure(Configuration eConfig);

    void SetMultiplexingMode(MultiplexingMode eMode);

    constexpr operator unsigned()
    {
        return m_uGlobalId;
    }

    constexpr bool operator==(const Pin& rhs) const
    {
        return m_uGlobalId == rhs.m_uGlobalId;
    }

    constexpr Port GetPort() const
    {
        return m_ePort;
    }

    constexpr pin_id_t GetLocalId() const
    {
        return m_localId;
    }

private:
    volatile RegisterGuard<PortGroup>* GetPortGroup();

private:
    unsigned m_uGlobalId;
    Port m_ePort;
    pin_id_t m_localId;
};

} // namespace SAMD
