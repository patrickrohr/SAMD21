/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock.h"
#include "config.h"
#include "gclk.h"
#include <samd21.h>
#include <stdbool.h>
#include <stdint.h>

static bool _clock_is_running(enum ClockSource eSource);
static unsigned _clock_get_frequency();


static bool _clock_is_running(enum ClockSource eSource)
{
    switch (eSource)
    {
    case eOSC32K:
        return SYSCTRL->PCLKSR.bit.OSC32KRDY;

    case eDFLL48M:
        return SYSCTRL->DFLLCTRL.bit.ENABLE;

    case eXOSC32K:
        // May not be necessary to check both. But this is a little finicky, so can't hurt.
        return SYSCTRL->XOSC32K.bit.ENABLE && SYSCTRL->PCLKSR.bit.XOSC32KRDY;

    default:
        return false;
    }
}

// TODO: function to get frequency
unsigned _clock_get_frequency();


void clock_init()
{
    // TODO: TMaybe move into pm.c, once there is one
    PM->APBAMASK.reg |= PM_APBAMASK_SYSCTRL;
}

void clock_osc32k_start()
{
#ifdef CONFIG_OSC32K_ENABLED
    if (_clock_is_running(eOSC32K))
    {
        return;
    }

# ifdef CONFIG_OSC32K_CALIB_USE_FIXED_VAL
    uint32_t uCalib = CONFIG_OSC32K_CALIB;
# else
    uint32_t uCalib = (*((uint32_t*)FUSES_OSC32K_CAL_ADDR) & FUSES_OSC32K_CAL_Msk) >> FUSES_OSC32K_CAL_Pos;
# endif

    // TODO: compile time configure these values
    SYSCTRL_OSC32K_Type objRegisterTmp =
    {
        .bit.ENABLE   = 1,
        .bit.EN32K    = 1,
        .bit.CALIB    = uCalib,
        .bit.STARTUP  = CONFIG_OSC32K_STARTUP,
        .bit.WRTLOCK  = CONFIG_OSC32K_WRTLOCK,
        .bit.ONDEMAND = CONFIG_OSC32K_ONDEMAND,
        .bit.RUNSTDBY = CONFIG_OSC32K_RUNSTDBY,
    };

    SYSCTRL->OSC32K = objRegisterTmp;
    while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC32KRDY));
#else
    assert(0);
#endif
}

void clock_osc32k_stop()
{
    // Just reset it completely
    SYSCTRL->OSC32K.reg = 0;
}

void clock_dfll48m_start()
{
#ifdef CONFIG_DFLL48M_ENABLED
    if (_clock_is_running(eDFLL48M))
    {
        return;
    }

    // Flash wait states to support 48MHz
    NVMCTRL->CTRLB.bit.RWS = 1;

    // Errata
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

    // TODO: If we are running open loop, set DFLLVAL

    // Set DFLL Multiplier
    SYSCTRL_DFLLMUL_Type objDfllMulTmp =
    {
        .bit.MUL   = 1464, // TODO: Calculate
# ifdef CONFIG_DFLL48M_CLOSED_LOOP
        .bit.CSTEP = CONFIG_DFLL48M_MUL_CSTEP,
        .bit.FSTEP = CONFIG_DFLL48M_MUL_FSTEP
# endif
    };
    SYSCTRL->DFLLMUL = objDfllMulTmp;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

    // Set DFLL Control
    SYSCTRL_DFLLCTRL_Type objDfllCtrlTmp =
    {
        // Open / Closed Loop
        .bit.RUNSTDBY = CONFIG_DFLL48M_CTRL_RUNSTDBY,
        .bit.ONDEMAND = CONFIG_DFLL48M_CTRL_ONDEMAND,
# ifdef CONFIG_DFLL48M_CLOSED_LOOP
        .bit.MODE     = CONFIG_DFLL48M_CTRL_MODE,
        .bit.WAITLOCK = CONFIG_DFLL48M_CTRL_WAITLOCK,
        .bit.QLDIS    = CONFIG_DFLL48M_CTRL_QLDIS,
        .bit.CCDIS    = CONFIG_DFLL48M_CTRL_CCDIS,
        .bit.BPLCKC   = CONFIG_DFLL48M_CTRL_BPLCKC,
        .bit.LLAW     = CONFIG_DFLL48M_CTRL_LLAW,
        .bit.STABLE   = CONFIG_DFLL48M_CTRL_STABLE,
# endif
    };
    SYSCTRL->DFLLCTRL = objDfllCtrlTmp;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

    // Enable DFLL
    SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE;

    // Wait for locks and DFLL Ready
# ifdef CONFIG_DFLL48M_CLOSED_LOOP
    while (!SYSCTRL->PCLKSR.bit.DFLLLCKC
           && !SYSCTRL->PCLKSR.bit.DFLLLCKF);
# endif
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
#else
    assert(0);
#endif
}

void clock_dfll48m_stop()
{
    // May not be necessary, and may even be wrong. Possibly this should be inside glck
    // and be dependent on the clock output frequency and cpu bus divider
    // TODO: consider checking if this is set to 1 first.
    NVMCTRL->CTRLB.bit.RWS = 0;

    SYSCTRL->DFLLCTRL.reg = 0;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
}

void clock_xosc32k_start()
{
    if (_clock_is_running(eXOSC32K))
    {
        return;
    }

    SYSCTRL_XOSC32K_Type objXosc32kTmp =
    {
        .bit.STARTUP  = 0x6,
        .bit.XTALEN   = 1,
        .bit.EN32K    = 1,
        .bit.RUNSTDBY = 1
    };
    SYSCTRL->XOSC32K = objXosc32kTmp;
    // Separate write to Enable bit as per Datasheet
    SYSCTRL->XOSC32K.bit.ENABLE = 1;
    while (!SYSCTRL->PCLKSR.bit.XOSC32KRDY);
}

void clock_xosc32k_stop()
{
    SYSCTRL->XOSC32K.bit.ENABLE = 0;
}
