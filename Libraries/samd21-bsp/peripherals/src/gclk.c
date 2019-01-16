/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "config.h"
#include "gclk.h"
#include "error.h"
#include <stdbool.h>
#include <samd21.h>

/*!************************************************************
 * HACK to find number of GCLKs as there is no point having more
 * GCLKs than enabled Clock Sources.
 * TODO: Is there a less "hacky" way to do this?
**************************************************************/
enum
{
#if CONFIG_OSC32K_ENABLED
    _osc32k,
#endif
#if CONFIG_DFLL48M_ENABLED
    _dfll48m,
#endif
#if CONFIG_XOSC32K_ENABLED
    _xosc32K,
#endif
#if CONFIG_OSC8M_ENABLED
    _osc8m,
#endif

    ClockCount
};
// TODO: There *may* be a use case for just 1 clock
static_assert(ClockCount >= 2, "We need at least 2 clocks to operate.");

/*!************************************************************
 * Generic Clock Controller Handle
**************************************************************/
typedef struct
{
    uint32_t m_uGclkGenCtrlFlags; // Generator Control Flags
    uint16_t m_uGclkGenDiv;       // Division Factor
    uint16_t m_uClockSource;      // Clock source
} Gclk_t;

// Static Data
static Gclk_t g_GclkHandleArr[ClockCount] = { { 0 } };

// Private Functions
static void _gclk_handle_init();
static void _gclk_configure(uint8_t uId, enum ClockSource eClockSource);
static void _gclk_clock_start(enum ClockSource eClockSource);


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
    g_GclkHandleArr[uId].m_uClockSource      = eClockSource;
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

    case eOSC8M:
        clock_osc8m_start();
        break;

    default:
        assert(0);
        break;
    }
}

void gclk_clock_stop(enum ClockSource eClockSource)
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

    case eOSC8M:
        clock_osc8m_stop();
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
        .bit.SRC   = g_GclkHandleArr[uId].m_uClockSource,
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
