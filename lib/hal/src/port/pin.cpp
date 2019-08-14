/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "port/pin.hpp"
#include <samd21.h>

namespace SAMD
{

auto reg_PM       = MakeRegisterGuard(PM);
auto reg_APBBMASK = MakeRegisterGuard(&reg_PM->Get().APBBMASK);

auto reg_PORT = MakeRegisterGuard(PORT);

void Pin::Configure(Configuration eConfig)
{
    volatile auto& portGroup = GetPortGroup()->Get();

    switch (eConfig)
    {
    case Pin::Configuration::eInputStandard:
        portGroup.PINCFG[m_localId.Get()].reg = PORT_PINCFG_INEN;
        portGroup.DIRCLR.reg                  = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eInputPull:
        portGroup.PINCFG[m_localId.Get()].reg =
            PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
        portGroup.DIRCLR.reg = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eTotemPoleDisabledIn:
        portGroup.PINCFG[m_localId.Get()].reg = 0;
        portGroup.DIRSET.reg                  = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eTotemPoleEnabledIn:
        portGroup.PINCFG[m_localId.Get()].reg = PORT_PINCFG_INEN;
        portGroup.DIRSET.reg                  = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eOutputPull:
        portGroup.PINCFG[m_localId.Get()].reg = PORT_PINCFG_PULLEN;
        portGroup.DIRCLR.reg                  = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eAnalog:
        portGroup.PINCFG[m_localId.Get()].reg = 0;
        portGroup.DIRCLR.reg                  = (1 << m_localId.Get());
        break;
    }
}

void Pin::SetMultiplexingMode(MultiplexingMode eMode)
{
    volatile auto& portGroup = GetPortGroup()->Get();

    portGroup.PINCFG[m_localId.Get()].reg |= PORT_PINCFG_PMUXEN;
    // TODO: Weirdly, I used to have this the other way round.
    // Double check if this doesn't work.
    portGroup.PMUX[m_localId.Get() / 2].reg |=
        (m_localId.Get() % 2) ? PORT_PMUX_PMUXE(static_cast<unsigned>(eMode))
                              : PORT_PMUX_PMUXO(static_cast<unsigned>(eMode));
}

volatile RegisterGuard<PortGroup>* Pin::GetPortGroup()
{
    return MakeRegisterGuard(
        &reg_PORT->Get().Group[static_cast<unsigned>(m_ePort)]);
}

} // namespace SAMD
