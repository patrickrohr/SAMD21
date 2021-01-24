/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <clock/clock_definitions.hpp>
#include <port/pin.hpp>

using namespace SAMD;

int main()
{
    ReferenceClock slowClock(gclk_id_t(1));
    slowClock.Enable();
    slowClock.WaitOnClockIsRunning();
    slowClock.AddOutput(ClockOutput::eGCLK_DFLL48M_REF);

    DfllClock fastClock(gclk_id_t(0), slowClock);
    fastClock.Enable();
    fastClock.WaitOnClockIsRunning();

    Pin ledPin(Pin::Port::ePortA, Pin::pin_id_t(17));
    ledPin.Configure(Pin::Configuration::eTotemPoleDisabledIn);
    ledPin.SetOutput();

    return 0;
}
