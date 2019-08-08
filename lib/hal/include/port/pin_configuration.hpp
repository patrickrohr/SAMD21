/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <common/id_traits.hpp>

namespace SAMD
{

class PinConfiguration;
using pin_id_t = id_traits<PinConfiguration>;

/**
 * @brief      Class for pin configuration.
 * @details    For now, this class just serves as a holder for pin configurations.
 *             In the future, I want to use this class to write multiple pin
 *             configurations to the port at once.
 */
class PinConfiguration
{
public:
    enum class PinDirection
    {
        eInput  = 0,
        eOutput = 1
    };

    enum class Type
    {
        eInputStandard,
        eInputPull,
        eTotemPoleDisabledIn, // Totem pole with disabled input
        eTotemPoleEnabledIn,  // Totem pole with enabled input
        eOutputPull,
        eAnalog,         // Analog I/O
        eReset = eAnalog // Reset state
    };

public:
    // TODO: move to cpp
    PinConfiguration(pin_id_t pin, Type type);

    pin_id_t GetPin() const;

    Type GetType() const;

private:
    pin_id_t m_pin;
    Type m_eType;
};

} // namespace SAMD
