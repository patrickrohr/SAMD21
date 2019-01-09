/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "gclk.h"
#include "error.h"
#include <samd21.h>

/*!************************************************************
 * Clock System Overview:
 *
 * --------------     -------------------     -----------------------
 * | CLK Source | >>> | Generic CLK Gen | >>> | Gen CLK Multiplexer |
 * --------------     -------------------     -----------------------
 *   (CLKCTRL)         (GENCTRL & GENDIV)
**************************************************************/

// TODO: Consider moving sysctrl functions into a separate file.
void _dfll_ctrl_sync()
{
    while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0);    // Wait for DFLL synchronization complete
}

unsigned g_uCpuFrequency = 48000;
int _sysctrl_init_dfll48()
{
    // See Errata 9905 - write to DFLLCTRL before using it
    SYSCTRL->DFLLCTRL.reg = 0;
    _dfll_ctrl_sync();

    SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL(g_uCpuFrequency);

    // TODO: make these values compile time options
    // Set DFLL for USB Clock Recovery Mode, Bypass Coarse Lock, Disable Chill Cycle,
    //   Fine calibration register locks (stable) after fine lock
    SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE | SYSCTRL_DFLLCTRL_USBCRM |
                            SYSCTRL_DFLLCTRL_BPLCKC | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_STABLE;
    _dfll_ctrl_sync();
    return 0;
}

void gclk_init(Gclk_t* self, uint8_t uId, enum ClockSource eGclkSource)
{
    assert(self);
    // TODO: This could check for nullptr and set default values for all parameters.
    // Alternatively, init could literally just initialize all variables, and a configure function could be used to set values or use defaults.
    // This would make sense if value sets are unique for each GclkId
    self->m_uGclkId           = uId;
    self->m_uGclkSource       = eGclkSource;
    self->m_uGclkGenDiv       = 0;
    self->m_uGclkGenCtrlFlags = 0;

    _gclk_get_defaults(self, eGclkSource);
}

void _gclk_get_defaults(Gclk_t* self, enum ClockSource eGclkSource)
{
    // Init
    self->m_uGclkGenDiv = 1; // TODO: What should the global default be? compile flag??

    switch (eGclkSource)
    {
    case eDFLL48M:
        self->m_uGclkGenCtrlFlags |= GCLK_GENCTRL_RUNSTDBY;
        break;

    default:
        // do nothing
        break;
    }

    // The generic clock generator duty cycle is not 50/50 for odd division factors
    if (self->m_uGclkGenDiv % 2)
    {
        self->m_uGclkGenCtrlFlags |= GCLK_GENCTRL_IDC;
    }
}

void gclk_enable(const Gclk_t* self)
{
    assert(self);

    // TODO: make sure clock source is on
    switch (self->m_uGclkSource)
    {
    case eDFLL48M:
        // TODO: sysctrl_init(eDFLL48M)
        _sysctrl_init_dfll48();
        break;

    default:
        assert(0);
        break;
    }

    GCLK->GENDIV.reg  = GCLK_GENDIV_ID(self->m_uGclkId) | GCLK_GENDIV_DIV(self->m_uGclkGenDiv);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(self->m_uGclkId) | GCLK_GENCTRL_SRC(self->m_uGclkSource) |
                        self->m_uGclkGenCtrlFlags | GCLK_GENCTRL_GENEN;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}
