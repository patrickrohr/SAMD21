/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/clock_base.hpp"
#include "port/pin.hpp"
#include <common/id_traits.hpp>
#include <common/vector.hpp>
#include <cstdint>
#include <hal_utils/register.hpp>
#include <samd21.h>

namespace SAMD
{

class SercomBase;
using sercom_id_t = id_traits<SercomBase, uint8_t>;

/**
 * @brief      Class for serial port.
 * @details    The idea is for this to be the base class for all serial
 *             ports.
 */
class SercomBase
{
public:
    enum class Mode : uint8_t
    {
        eSercomUsartExternalClk,
        eSercomUsartInternalClk,
        eSercomSpiSlave,
        eSercomSpiMaster,
        eSercomI2cSlave,
        eSercomI2cMaster,
    };

protected:
    struct SercomPinConfig
    {
        using pad_number_t = unsigned;

        SercomPinConfig(
            Pin pin,
            Pin::MultiplexingMode eMode,
            sercom_id_t sercomId,
            pad_number_t padNumber) :
            PadNumber(padNumber),
            PinId(pin),
            MultiplexingMode(eMode),
            SercomId(sercomId)
        {
        }

        pad_number_t PadNumber;
        Pin PinId;
        Pin::MultiplexingMode MultiplexingMode;
        sercom_id_t SercomId;
    };

public:
    SercomBase(sercom_id_t id, Mode eMode, ClockBase& sourceClock);
    virtual ~SercomBase() = 0;

protected:
    volatile RegisterGuard<Sercom>* GetRegister();
    sercom_id_t GetId() const;
    const Vector<SercomPinConfig, 62>& GetPinConfigurations() const;

private:
    sercom_id_t m_id;
    Mode m_eMode;
};

} // namespace SAMD
