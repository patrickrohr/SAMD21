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
    volatile auto& pPortGroup = GetPortGroup()->Get();

    switch (eConfig)
    {
    case Pin::Configuration::eInputStandard:
        pPortGroup.PINCFG[m_localId.Get()].reg = PORT_PINCFG_INEN;
        pPortGroup.DIRCLR.reg                    = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eInputPull:
        pPortGroup.PINCFG[m_localId.Get()].reg =
            PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
        pPortGroup.DIRCLR.reg = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eTotemPoleDisabledIn:
        pPortGroup.PINCFG[m_localId.Get()].reg = 0;
        pPortGroup.DIRSET.reg                    = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eTotemPoleEnabledIn:
        pPortGroup.PINCFG[m_localId.Get()].reg = PORT_PINCFG_INEN;
        pPortGroup.DIRSET.reg                    = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eOutputPull:
        pPortGroup.PINCFG[m_localId.Get()].reg = PORT_PINCFG_PULLEN;
        pPortGroup.DIRCLR.reg                    = (1 << m_localId.Get());
        break;

    case Pin::Configuration::eAnalog:
        pPortGroup.PINCFG[m_localId.Get()].reg = 0;
        pPortGroup.DIRCLR.reg                    = (1 << m_localId.Get());
        break;
    }
}

void Pin::SetMultiplexingMode(MultiplexingMode eMode)
{

    // first pincfg.pmuxen
    //
}

volatile RegisterGuard<PortGroup>* Pin::GetPortGroup()
{
    return MakeRegisterGuard(
        &reg_PORT->Get().Group[static_cast<unsigned>(m_ePort)]);
}

} // namespace SAMD
