/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "serial/sercom_base.hpp"
#include "port/pin.hpp"
#include <common/error.hpp>
#include <common/vector.hpp>
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

SercomBase::SercomBase(sercom_id_t id, Mode eMode, ClockBase& sourceClock) :
    m_id(id), m_eMode(eMode)
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

    // TODO: Do I really need this?
    reg_PM->Get().APBCMASK.reg |= PM_APBCMASK_PAC2;

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
        // The SAMD21G18A only has 4 serial communication interfaces.
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

sercom_id_t SercomBase::GetId() const
{
    return m_id;
}

const Vector<SercomBase::SercomPinConfig, 62>&
SercomBase::GetPinConfigurations() const
{
    static bool isInitialized = false;
    static Vector<SercomPinConfig, 62> vPinConfig;

    if (!isInitialized)
    {
        isInitialized = true;

        auto PA = [](unsigned pinId) {
            return Pin(Pin::Port::ePortA, static_cast<Pin::pin_id_t>(pinId));
        };

        auto PB = [](unsigned pinId) {
            return Pin(Pin::Port::ePortB, static_cast<Pin::pin_id_t>(pinId));
        };

        auto SercomMode =
            [](const Pin& pin, unsigned sercomId, unsigned padNumber) {
                return SercomPinConfig(
                    pin,
                    Pin::MultiplexingMode::eSercom,
                    static_cast<sercom_id_t>(sercomId),
                    padNumber);
            };

        auto SercomAltMode =
            [](const Pin& pin, unsigned sercomId, unsigned padNumber) {
                return SercomPinConfig(
                    pin,
                    Pin::MultiplexingMode::eSercomAlt,
                    static_cast<sercom_id_t>(sercomId),
                    padNumber);
            };

        // 0
        vPinConfig.PushBack(SercomAltMode(PA(0), 1, 0));
        vPinConfig.PushBack(SercomAltMode(PA(1), 1, 1));
        vPinConfig.PushBack(SercomAltMode(PB(8), 4, 0));
        vPinConfig.PushBack(SercomAltMode(PB(9), 4, 1));
        vPinConfig.PushBack(SercomAltMode(PA(4), 0, 0));
        vPinConfig.PushBack(SercomAltMode(PA(5), 0, 1));
        vPinConfig.PushBack(SercomAltMode(PA(6), 0, 2));
        vPinConfig.PushBack(SercomAltMode(PA(7), 0, 3));
        vPinConfig.PushBack(SercomAltMode(PA(8), 2, 0));
        vPinConfig.PushBack(SercomAltMode(PA(9), 2, 1));
        // 10
        vPinConfig.PushBack(SercomAltMode(PA(10), 2, 2));
        vPinConfig.PushBack(SercomAltMode(PA(11), 2, 3));
        vPinConfig.PushBack(SercomAltMode(PB(10), 4, 2));
        vPinConfig.PushBack(SercomAltMode(PB(11), 4, 3));
        vPinConfig.PushBack(SercomAltMode(PA(12), 2, 0));
        vPinConfig.PushBack(SercomAltMode(PA(13), 2, 1));
        vPinConfig.PushBack(SercomAltMode(PA(14), 2, 2));
        vPinConfig.PushBack(SercomAltMode(PA(15), 2, 3));
        vPinConfig.PushBack(SercomAltMode(PA(16), 3, 0));
        vPinConfig.PushBack(SercomAltMode(PA(17), 3, 1));
        // 20
        vPinConfig.PushBack(SercomAltMode(PA(18), 3, 2));
        vPinConfig.PushBack(SercomAltMode(PA(19), 3, 3));
        vPinConfig.PushBack(SercomAltMode(PA(20), 3, 2));
        vPinConfig.PushBack(SercomAltMode(PA(21), 3, 3));
        vPinConfig.PushBack(SercomAltMode(PA(22), 5, 0));
        vPinConfig.PushBack(SercomAltMode(PA(23), 5, 1));
        vPinConfig.PushBack(SercomAltMode(PA(24), 5, 2));
        vPinConfig.PushBack(SercomAltMode(PA(25), 5, 3));
        vPinConfig.PushBack(SercomAltMode(PB(22), 5, 2));
        vPinConfig.PushBack(SercomAltMode(PB(23), 5, 3));
        // 30
        vPinConfig.PushBack(SercomAltMode(PA(30), 1, 2));
        vPinConfig.PushBack(SercomAltMode(PA(31), 1, 3));
        vPinConfig.PushBack(SercomAltMode(PB(30), 5, 0));
        vPinConfig.PushBack(SercomAltMode(PB(31), 5, 1));
        vPinConfig.PushBack(SercomAltMode(PB(0), 5, 2));
        vPinConfig.PushBack(SercomAltMode(PB(1), 5, 3));
        vPinConfig.PushBack(SercomAltMode(PB(2), 5, 0));
        vPinConfig.PushBack(SercomAltMode(PB(3), 5, 1));
        vPinConfig.PushBack(SercomMode(PA(8), 0, 0));
        vPinConfig.PushBack(SercomMode(PA(9), 0, 1));
        // 40
        vPinConfig.PushBack(SercomMode(PA(10), 0, 2));
        vPinConfig.PushBack(SercomMode(PA(11), 0, 3));
        vPinConfig.PushBack(SercomMode(PB(12), 4, 0));
        vPinConfig.PushBack(SercomMode(PB(13), 4, 1));
        vPinConfig.PushBack(SercomMode(PB(14), 4, 2));
        vPinConfig.PushBack(SercomMode(PB(15), 4, 3));
        vPinConfig.PushBack(SercomMode(PA(12), 2, 0));
        vPinConfig.PushBack(SercomMode(PA(13), 2, 1));
        vPinConfig.PushBack(SercomMode(PA(14), 2, 2));
        vPinConfig.PushBack(SercomMode(PA(15), 2, 3));
        // 50
        vPinConfig.PushBack(SercomMode(PA(16), 1, 0));
        vPinConfig.PushBack(SercomMode(PA(17), 1, 1));
        vPinConfig.PushBack(SercomMode(PA(18), 1, 2));
        vPinConfig.PushBack(SercomMode(PA(19), 1, 3));
        vPinConfig.PushBack(SercomMode(PB(16), 5, 0));
        vPinConfig.PushBack(SercomMode(PB(17), 5, 1));
        vPinConfig.PushBack(SercomMode(PA(20), 5, 2));
        vPinConfig.PushBack(SercomMode(PA(21), 5, 3));
        vPinConfig.PushBack(SercomMode(PA(22), 3, 0));
        vPinConfig.PushBack(SercomMode(PA(23), 3, 1));
        // 60
        vPinConfig.PushBack(SercomMode(PA(24), 3, 2));
        vPinConfig.PushBack(SercomMode(PA(25), 3, 3));
        // 62
    }

    return vPinConfig;
}

} // namespace SAMD
