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
#include <stdint.h>


void clock_init(enum ClockSource eSource)
{
    switch (eSource)
    {
#if CONFIG_OSC32K_ENABLED
    case eOSC32K:
    {
        SYSCTRL->OSC32K.bit.CALIB    = FUSES_OSC32K_CAL(*(uint32_t*)FUSES_OSC32K_CAL_ADDR);
        SYSCTRL->OSC32K.bit.WRTLOCK  = CONFIG_OSC32K_WRITELOCK;
        SYSCTRL->OSC32K.bit.STARTUP  = CONFIG_OSC32K_STARTUP;
        SYSCTRL->OSC32K.bit.ONDEMAND = CONFIG_OSC32K_ONDEMAND;
        SYSCTRL->OSC32K.bit.RUNSTDBY = CONFIG_OSC32K_RUNSTANDBY;
        SYSCTRL->OSC32K.bit.ENABLE   = 1;
        SYSCTRL->OSC32K.bit.EN32K    = 1; // Should I wait to enable this until after RDY?

        while (!SYSCTRL->PCLKSR.bit.OSC32KRDY);
        break;
    }
#endif
#if CONFIG_DFLL48M_ENABLED
    case eDFLL48M:
    {
        // TODO: Make this configurable similar to OSC32K above.

        // See Errata 9905 - write to DFLLCTRL before using it
        SYSCTRL->DFLLCTRL.reg = 0;
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

        // 48000 because USBCRM expects a 1000Hz pulse. This needs to be fixed
        SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL(48000);

        // TODO: make these values compile time options
        // Set DFLL for USB Clock Recovery Mode, Bypass Coarse Lock, Disable Chill Cycle,
        // Fine calibration register locks (stable) after fine lock
        SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE | SYSCTRL_DFLLCTRL_USBCRM |
                                SYSCTRL_DFLLCTRL_BPLCKC | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_STABLE;
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
        break;
    }
#endif
    default:
        assert(0);
        break;
    }
}
