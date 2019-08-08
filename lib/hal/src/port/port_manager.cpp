/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "port/port_manager.hpp"
#include <samd21.h>

namespace SAMD
{

auto reg_PM       = MakeRegisterGuard(PM);
auto reg_APBBMASK = MakeRegisterGuard(&reg_PM->Get().APBBMASK);

auto reg_PORT = MakeRegisterGuard(PORT);

PortManager::PortManager(Group eGroup) :
    m_regPortGroup(MakeRegisterGuard(&reg_PORT->Get().Group[static_cast<unsigned>(eGroup)])),
    m_ePortGroup(eGroup)
{
    // Find a better way to do this
    // There should probably be a PowerManager, and every peripheral should
    // identify itself. Then we could to PowerManager::Enable(*this);
    reg_APBBMASK->Get().reg |= PM_APBBMASK_PORT;
}

void PortManager::ConfigurePin(const PinConfiguration& objConfig)
{
    switch (objConfig.GetType())
    {
    case PinConfiguration::Type::eInputStandard:
        m_regPortGroup->Get().PINCFG[objConfig.GetPin().Get()].reg = PORT_PINCFG_INEN;
        m_regPortGroup->Get().DIRCLR.reg = (1 << objConfig.GetPin().Get());
        break;

    case PinConfiguration::Type::eInputPull:
        m_regPortGroup->Get().PINCFG[objConfig.GetPin().Get()].reg =
            PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
        m_regPortGroup->Get().DIRCLR.reg = (1 << objConfig.GetPin().Get());
        break;

    case PinConfiguration::Type::eTotemPoleDisabledIn:
        m_regPortGroup->Get().PINCFG[objConfig.GetPin().Get()].reg = 0;
        m_regPortGroup->Get().DIRSET.reg = (1 << objConfig.GetPin().Get());
        break;

    case PinConfiguration::Type::eTotemPoleEnabledIn:
        m_regPortGroup->Get().PINCFG[objConfig.GetPin().Get()].reg = PORT_PINCFG_INEN;
        m_regPortGroup->Get().DIRSET.reg = (1 << objConfig.GetPin().Get());
        break;

    case PinConfiguration::Type::eOutputPull:
        m_regPortGroup->Get().PINCFG[objConfig.GetPin().Get()].reg =
            PORT_PINCFG_PULLEN;
        m_regPortGroup->Get().DIRCLR.reg = (1 << objConfig.GetPin().Get());
        break;

    case PinConfiguration::Type::eAnalog:
        m_regPortGroup->Get().PINCFG[objConfig.GetPin().Get()].reg = 0;
        m_regPortGroup->Get().DIRCLR.reg = (1 << objConfig.GetPin().Get());
        break;
    }
}

void PortManager::SetPinOutput(pin_id_t pin)
{
    m_regPortGroup->Get().OUTSET.reg = (1 << pin.Get());
}

void PortManager::ClearPinOutput(pin_id_t pin)
{
    m_regPortGroup->Get().OUTCLR.reg = (1 << pin.Get());
}

void PortManager::TogglePinOutput(pin_id_t pin)
{
    m_regPortGroup->Get().OUTTGL.reg = (1 << pin.Get());
}

int PortManager::ReadPinInput(pin_id_t pin)
{
    return m_regPortGroup->Get().IN.reg & (1 << pin.Get());
}

} // namespace SAMD
