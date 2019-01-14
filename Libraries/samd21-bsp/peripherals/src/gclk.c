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

// Static Data
static Gclk_t g_GclkHandleArr[ClockCount];

// Private Functions
void _gclk_configure(uint8_t uId, enum ClockSource eClockSource);
void _gclk_clock_start(enum ClockSource eClockSource);
void _gclk_clock_stop(enum ClockSource eClockSource);


void gclk_init()
{
    // TODO: this can be moved into PM once that exists.
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

    // software reset to make sure we start from a clean state
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    while ((GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY));

    // Initialize clocks
    clock_init();
}

// TODO: Maybe uId should be an enum?
void _gclk_configure(uint8_t uId, enum ClockSource eClockSource)
{
    g_GclkHandleArr[uId].m_uGclkSource       = eClockSource;
    g_GclkHandleArr[uId].m_uGclkGenDiv       = 1;
    g_GclkHandleArr[uId].m_uGclkGenCtrlFlags = 0;

    switch (eClockSource)
    {
    case eOSC32K:
    case eDFLL48M:
        g_GclkHandleArr[uId].m_uGclkGenCtrlFlags |= GCLK_GENCTRL_RUNSTDBY;
        break;

    default:
        // do nothing
        break;
    }

    // The generic clock generator duty cycle is not 50/50 for odd division factors
    if (g_GclkHandleArr[uId].m_uGclkGenDiv % 2)
    {
        g_GclkHandleArr[uId].m_uGclkGenCtrlFlags |= GCLK_GENCTRL_IDC;
    }
}

void _gclk_clock_start(enum ClockSource eClockSource)
{
    switch (eClockSource)
    {
    case eOSC32K:
        clock_osc32k_start();
        break;

    case eXOSC32K:
        clock_xosc32k_start();
        break;

    case eDFLL48M:
        clock_dfll48m_start();
        break;

    default:
        assert(0);
        break;
    }
}

void _gclk_clock_stop(enum ClockSource eClockSource)
{
    switch (eClockSource)
    {
    case eOSC32K:
        clock_osc32k_stop();
        break;

    case eXOSC32K:
        clock_xosc32k_stop();
        break;

    case eDFLL48M:
        clock_dfll48m_stop();
        break;

    default:
        assert(0);
        break;
    }
}

void gclk_set_input(uint8_t uId, enum ClockSource eClockSource)
{
    assert(uId < ClockCount);
    assert(eClockSource < GCLK_SOURCE_NUM);

    _gclk_clock_start(eClockSource);
    _gclk_configure(uId, eClockSource);

    // Setup Generic Clock Generator Division
    GCLK_GENDIV_Type objGeneratorDivision =
    {
        .bit.ID  = uId,
        .bit.DIV = g_GclkHandleArr[uId].m_uGclkGenDiv
    };
    GCLK->GENDIV = objGeneratorDivision;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

    // Setup Generic Clock Generator Control
    GCLK_GENCTRL_Type objGeneratorControl =
    {
        .bit.ID    = uId,
        .bit.SRC   = g_GclkHandleArr[uId].m_uGclkSource,
        .bit.GENEN = 1
    };
    objGeneratorControl.reg |= g_GclkHandleArr[uId].m_uGclkGenCtrlFlags;
    GCLK->GENCTRL            = objGeneratorControl;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}

void gclk_add_output(uint8_t uId, enum ClockOutput eClockOutput)
{
    assert(uId < ClockCount);
    assert(eClockOutput < _eClockOutputMax);

    // Set the Generic Clock Output
    GCLK_CLKCTRL_Type objClockControl =
    {
        .bit.ID    = eClockOutput,
        .bit.GEN   = uId,
        .bit.CLKEN = 1
    };
    GCLK->CLKCTRL = objClockControl;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}
