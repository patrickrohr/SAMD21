/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "port/pin_configuration.hpp"
#include "hal_utils/register.hpp"
#include <samd21.h>

namespace SAMD
{

// Cannot name this port, because CMSIS already has a Port class.
class PortManager
{
public:
    enum class Group
    {
        ePortA = 0,
        ePortB = 1
    };

public:
    PortManager(Group eGroup);
    void ConfigurePin(const PinConfiguration& objConfig);

    void SetPinOutput(pin_id_t pin);
    void ClearPinOutput(pin_id_t pin);
    void TogglePinOutput(pin_id_t pin);
    int ReadPinInput(pin_id_t pin);

private:
    volatile RegisterGuard<PortGroup>* m_regPortGroup;
    Group m_ePortGroup;
};

} // namespace SAMD
