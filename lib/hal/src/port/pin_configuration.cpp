/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "port/pin_configuration.hpp"

namespace SAMD
{

PinConfiguration::PinConfiguration(pin_id_t pin, Type type) :
    m_pin(pin),
    m_eType(type)
{
}

pin_id_t PinConfiguration::GetPin() const
{
    return m_pin;
}

PinConfiguration::Type PinConfiguration::GetType() const
{
    return m_eType;
}

} // namespace SAMD
