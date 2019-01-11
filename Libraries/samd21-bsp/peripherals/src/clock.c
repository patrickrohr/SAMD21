/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "config.h"
#include "clock.h"
#include "error.h"
#include <samd21.h>
#include <stdbool.h>
#include <stdint.h>

static bool _clock_is_enabled(enum ClockSource eSource);


static bool _clock_is_enabled(enum ClockSource eSource)
{
    switch (eSource)
    {
    case eOSC32K:
        return 1 == SYSCTRL->OSC32K.bit.ENABLE;

    case eDFLL48M:
        return 1 == SYSCTRL->DFLLCTRL.bit.ENABLE;

    default:
        return false;
    }
}

void clock_init(enum ClockSource eSource)
{
    // If the clock is already enabled, there is nothing to do
    if (_clock_is_enabled(eSource))
    {
        return;
    }

    switch (eSource)
    {
#if CONFIG_OSC32K_ENABLED
    case eOSC32K:
    {
        // TODO: Exercise for tomorrow: use internal 32k oscillator as a source for DFLL48M
        uint32_t uCalib = (*((uint32_t*)FUSES_OSC32K_CAL_ADDR) & FUSES_OSC32K_CAL_Msk) >> FUSES_OSC32K_CAL_Pos;

        // TODO: Make this one atomic 32bit write
        // TODO: in config.h, prefer a generic CONFIG_XYZ_REG_FLAGS to keep it simpler
        SYSCTRL->OSC32K.bit.CALIB = uCalib;

        SYSCTRL->OSC32K.bit.WRTLOCK  = CONFIG_OSC32K_WRITELOCK;
        SYSCTRL->OSC32K.bit.STARTUP  = CONFIG_OSC32K_STARTUP;
        SYSCTRL->OSC32K.bit.ONDEMAND = CONFIG_OSC32K_ONDEMAND;
        SYSCTRL->OSC32K.bit.RUNSTDBY = CONFIG_OSC32K_RUNSTANDBY;

        SYSCTRL->OSC32K.bit.EN32K  = 1;
        SYSCTRL->OSC32K.bit.ENABLE = 1;

        while (!SYSCTRL->PCLKSR.bit.OSC32KRDY);
        break;
    }
#endif
#if CONFIG_DFLL48M_ENABLED
    case eDFLL48M:
    {
// TODO!!!!!!!
//         // See Errata 9905 - write to DFLLCTRL before using it
//         SYSCTRL->DFLLCTRL.reg = 0;
//         while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

//         // Open-Loop Config
//         uint32_t uCoarse = (*((uint32_t*)FUSES_DFLL48M_COARSE_CAL_ADDR) & FUSES_DFLL48M_COARSE_CAL_Msk) >> FUSES_DFLL48M_COARSE_CAL_Pos;
//         uint32_t uFine   = (*((uint32_t*)FUSES_DFLL48M_FINE_CAL_ADDR) & FUSES_DFLL48M_FINE_CAL_Msk) >> FUSES_DFLL48M_FINE_CAL_Pos;
//         SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(uCoarse) | SYSCTRL_DFLLVAL_FINE(uFine);

// # if 1 == CONFIG_DFLL48M_OPENLOOP
//         // TODO: Enable...
// # endif

//         // 48000 because USBCRM expects a 1000Hz pulse. This needs to be fixed
//         SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL(48000);

//         // TODO: make these values compile time options
//         // Set DFLL for USB Clock Recovery Mode, Bypass Coarse Lock, Disable Chill Cycle,
//         // Fine calibration register locks (stable) after fine lock
//         SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE | SYSCTRL_DFLLCTRL_USBCRM |
//                                 SYSCTRL_DFLLCTRL_BPLCKC | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_STABLE;
//         while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
        break;
    }
#endif
    default:
        assert(0);
        break;
    }
}
