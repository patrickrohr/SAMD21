/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "serial/sercom_base.hpp"
#include <common/error.hpp>
#include <hal_utils/register.hpp>
#include <samd21.h>

namespace SAMD
{

volatile RegisterGuard<Sercom>* reg_SERCOM[SERCOM_INST_NUM] = {
    MakeRegisterGuard(SERCOM0),
    MakeRegisterGuard(SERCOM1),
    MakeRegisterGuard(SERCOM2),
    MakeRegisterGuard(SERCOM3)
};

auto reg_PM = MakeRegisterGuard(PM);

SercomBase::SercomBase(
    sercom_id_t id,
    Mode eMode,
    ClockSourceGeneric& sourceClock) :
    m_id(id),
    m_eMode(eMode)
{
    samd_assert(
        m_id.Get() < SERCOM_INST_NUM,
        "Sercom ID %u is out of bounds.",
        m_id.Get());
    samd_assert(
        sourceClock.IsEnabled(),
        "Sercom needs a running clock. Clock is disabled.");

    if (sourceClock.WaitOnClockIsRunning())
    {
        // error
        samd_assert(false, "Clock never started");
    }

    // TODO: setup port

    // Route clock output to peripheral
    switch (m_id.Get())
    {
    case 0:
        reg_PM->Get().APBCMASK.reg |= PM_APBCMASK_SERCOM0;
        sourceClock.AddOutput(ClockOutput::eGCLK_SERCOM0_CORE);
        break;
    case 1:
        reg_PM->Get().APBCMASK.reg |= PM_APBCMASK_SERCOM1;
        sourceClock.AddOutput(ClockOutput::eGCLK_SERCOM1_CORE);
        break;
    case 2:
        reg_PM->Get().APBCMASK.reg |= PM_APBCMASK_SERCOM2;
        sourceClock.AddOutput(ClockOutput::eGCLK_SERCOM2_CORE);
        break;
    case 3:
        reg_PM->Get().APBCMASK.reg |= PM_APBCMASK_SERCOM3;
        sourceClock.AddOutput(ClockOutput::eGCLK_SERCOM3_CORE);
        break;
    // TODO: There are more???
    case 4:
        reg_PM->Get().APBCMASK.reg |= PM_APBCMASK_SERCOM4;
        sourceClock.AddOutput(ClockOutput::eGCLK_SERCOM4_CORE);
        break;
    case 5:
        reg_PM->Get().APBCMASK.reg |= PM_APBCMASK_SERCOM5;
        sourceClock.AddOutput(ClockOutput::eGCLK_SERCOM5_CORE);
        break;
    }

    // TODO: Do I need the slow clock??
    sourceClock.AddOutput(ClockOutput::eGCLK_SERCOMx_SLOW);

    // Write MODE to CTRLA

    // Write 1 to enable bit in CTRLA
}

SercomBase::~SercomBase() = default;

volatile RegisterGuard<Sercom>* SercomBase::GetRegister()
{
    return reg_SERCOM[m_id.Get()];
}

} // namespace SAMD
