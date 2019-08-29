/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <common/id_traits.hpp>
#include <cstdint>
#include <hal_utils/register.hpp>
#include <samd21.h>

namespace SAMD
{

class Pin
{
public:
    using pin_id_t = id_traits<Pin, uint8_t>;

    enum class Port : uint8_t
    {
        ePortA   = 0,
        ePortB   = 1,
        eInvalid = 255
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

    enum class MultiplexingMode : uint8_t
    {
        eInvalid = 0,
        eModeA,
        eModeB,
        eModeC,
        eModeD,
        eModeE,
        eModeF,
        eModeG,
        eModeH,

        eEIC       = eModeA,
        eREF       = eModeB,
        eADC       = eModeB,
        eAC        = eModeB,
        ePTC       = eModeB,
        eDAC       = eModeB,
        eSercom    = eModeC,
        eSercomAlt = eModeD,
        eTC        = eModeE, // also TCC
        eTCC       = eModeF,
        eCOM       = eModeG,
        eGCLK      = eModeH, // also AC
    };

public:
    constexpr Pin(Port ePort, pin_id_t pinId) : m_localId(pinId), m_ePort(ePort)
    {
    }

    // You shouldn't use this directly, but I'll leave it public for now.
    void Configure(Configuration eConfig);

    void SetMultiplexingMode(MultiplexingMode eMode);

    constexpr operator unsigned()
    {
        return GetGlobalId();
    }

    constexpr bool operator==(const Pin& rhs) const
    {
        return GetGlobalId() == rhs.GetGlobalId();
    }

    constexpr bool operator<(const Pin& rhs) const
    {
        return GetGlobalId() < rhs.GetGlobalId();
    }

    constexpr Port GetPort() const
    {
        return m_ePort;
    }

    constexpr unsigned GetGlobalId() const
    {
        return m_localId.Get() + ((m_ePort == Port::ePortA) ? 32 : 0);
    }

    constexpr pin_id_t GetLocalId() const
    {
        return m_localId;
    }

private:
    volatile RegisterGuard<PortGroup>* GetPortGroup();

private:
    pin_id_t m_localId;
    Port m_ePort;
};

} // namespace SAMD
