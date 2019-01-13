/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "config.h"
#include "gclk.h"
#include "error.h"
#include <samd21.h>

/*!************************************************************
 * Clock System Overview:
 *
 * --------------     -------------------     -----------------------
 * | CLK Source | >>> | Generic CLK Gen | >>> | Gen CLK Multiplexer |
 * --------------     -------------------     -----------------------
 *   (SYSCTRL)         (GENCTRL & GENDIV)            (CLKCTRL)
**************************************************************/

enum
{
#if CONFIG_OSC32K_ENABLED
    _osc32k,
#endif
#if CONFIG_DFLL48M_ENABLED
    _dfll48m,
#endif

    ClockCount
};

/*!************************************************************
 * Generic Clock Controller Handle
**************************************************************/
typedef struct
{
    uint32_t m_uGclkGenCtrlFlags; // Generator Control Flags
    uint16_t m_uGclkGenDiv;       // Division Factor
    uint16_t m_uGclkSource;       // Clock source
} Gclk_t;

static Gclk_t g_arrGclk[ClockCount];


void gclk_init()
{
    // software reset to make sure we start from a clean state
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    while ((GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY));
}

// TODO: Maybe uId should be an enum?
void gclk_configure(uint8_t uId, enum ClockSource eGclkSource)
{
    assert(uId < ClockCount);

    g_arrGclk[uId].m_uGclkSource       = eGclkSource;
    g_arrGclk[uId].m_uGclkGenDiv       = 0;
    g_arrGclk[uId].m_uGclkGenCtrlFlags = 0;

    // Init
    g_arrGclk[uId].m_uGclkGenDiv = 1; // TODO: What should the global default be? compile flag??

    switch (eGclkSource)
    {
    case eOSC32K:
    case eDFLL48M:
        g_arrGclk[uId].m_uGclkGenCtrlFlags |= GCLK_GENCTRL_RUNSTDBY;
        break;

    default:
        // do nothing
        break;
    }

    // The generic clock generator duty cycle is not 50/50 for odd division factors
    if (g_arrGclk[uId].m_uGclkGenDiv % 2)
    {
        g_arrGclk[uId].m_uGclkGenCtrlFlags |= GCLK_GENCTRL_IDC;
    }
}

void gclk_enable_generator(uint8_t uId)
{
    assert(uId < ClockCount);

    // Initialize Clock Source
    clock_init(g_arrGclk[uId].m_uGclkSource);


    GCLK->GENDIV.reg = GCLK_GENDIV_ID(uId) |
                       GCLK_GENDIV_DIV(g_arrGclk[uId].m_uGclkGenDiv);

    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(uId) |
                        GCLK_GENCTRL_SRC(g_arrGclk[uId].m_uGclkSource) |
                        g_arrGclk[uId].m_uGclkGenCtrlFlags |
                        GCLK_GENCTRL_GENEN;

    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}

void gclk_enable_peripheral_channel(uint8_t uId, enum ClockPeripheral eClockPeripheral)
{
    assert(uId < ClockCount);
    assert(eClockPeripheral < _eINVALID);

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(eClockPeripheral) |
                        GCLK_CLKCTRL_GEN(uId) |
                        GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}
