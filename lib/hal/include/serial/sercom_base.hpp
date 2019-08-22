/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/clock_source_generic.hpp"
#include <common/id_traits.hpp>
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

public:
    SercomBase(sercom_id_t id, Mode eMode, ClockSourceGeneric& sourceClock);
    virtual ~SercomBase() = 0;

protected:
    volatile RegisterGuard<Sercom>* GetRegister();
    sercom_id_t GetId() const;

private:
    sercom_id_t m_id;
    Mode m_eMode;
};

} // namespace SAMD
