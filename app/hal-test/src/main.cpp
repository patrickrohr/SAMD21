/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <clock/xosc32k.hpp>
#include <clock/dfll48m.hpp>
#include <port/pin.hpp>

int main()
{
    SAMD::XOSC32K<> slowClock(SAMD::gclk_id_t(1));
    slowClock.Enable();
    slowClock.WaitOnClockIsRunning();
    slowClock.AddOutput(SAMD::ClockOutput::eGCLK_DFLL48M_REF);

    SAMD::DFLL48M<> fastClock(SAMD::gclk_id_t(0), slowClock);
    fastClock.Enable();
    fastClock.WaitOnClockIsRunning();

    SAMD::Pin ledPin(SAMD::Pin::Port::ePortA, SAMD::Pin::pin_id_t(17));
    ledPin.Configure(SAMD::Pin::Configuration::eTotemPoleDisabledIn);
    ledPin.SetOutput();

    return 0;
}
