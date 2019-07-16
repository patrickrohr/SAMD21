/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "gclk.h"
#include "clock_private.h"
#include "config.h"
#include "error.h"
#include <math.h>
#include <samd21.h>
#include <stdbool.h>
#include <stddef.h>


/*!************************************************************
 * Defines
**************************************************************/
#define GCLK_ID_NUM    9

/*!************************************************************
 * Generic Clock Controller Handle
**************************************************************/
typedef struct
{
    GCLK_GENCTRL_Type objInputControl;
    uint32_t          uDivisionFactor; // Calculate divider and GENCTRL Div selector value depending on what was set
    uint8_t           uGenericClockId;
} Gclk_t;

/*!************************************************************
 * Static Data
**************************************************************/
static int    g_arrClockSourceRefCount[GCLK_SOURCE_NUM] = { 0 };    // Needs to be initialized correctly. Consider hiding in function
static Gclk_t g_objHandleArray[GCLK_ID_NUM];

/*!************************************************************
 * Private Functions
**************************************************************/
static void _gclk_clock_start(enum ClockSource eClockSource);
static void _gclk_clock_stop(enum ClockSource eClockSource); // check ref count
static void _gclk_data_init(void);
static unsigned _gclk_get_dfll_input_frequency();

// static void _gclk_is_enabled(void);

void gclk_init(void)
{
    // TODO: this can be moved into PM once that exists.
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

    // software reset to make sure we start from a clean state
    // TODO: Re-enable?
    // GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    // while ((GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY));

    // Initialize static data members
    _gclk_data_init();

    // Initialize clocks
    clock_init();
}

void gclk_set_input(uint8_t uGenericClockId, enum ClockSource eClockSource)
{
    assert(uGenericClockId < GCLK_ID_NUM);
    assert(eClockSource < GCLK_SOURCE_NUM);

    Gclk_t* self = &g_objHandleArray[uGenericClockId];
    self->uGenericClockId         = uGenericClockId;
    self->objInputControl.bit.SRC = eClockSource;

    // Go ahead and start the clock since this can take a while
    _gclk_clock_start(eClockSource);
}

void gclk_set_division(uint8_t uGenericClockId, uint32_t uDivisionFactor)
{
    assert(uGenericClockId < GCLK_ID_NUM);
    // Check for:
    // Gen 0, 3-8: Max of 0xFF
    // Gen 1: Max of 0xFFFF
    // Gen 2: Max of 0xF
    // TODO: Do math and figure out GENCTRL Div Selector value
    // For now just save it raw
    g_objHandleArray[uGenericClockId].uDivisionFactor = uDivisionFactor;
}

void gclk_enable(uint8_t uGenericClockId)
{
    assert(uGenericClockId < GCLK_ID_NUM);

    Gclk_t* self = &g_objHandleArray[uGenericClockId];

    // Wait for clock source
    enum ClockSource eNewClockSource = self->objInputControl.bit.SRC;
    while (!clock_is_running(eNewClockSource));

    // Write to Gendiv
    GCLK_GENDIV_Type objGeneratorDivision =
    {
        .bit.ID  = uGenericClockId,
        .bit.DIV = self->uDivisionFactor
    };
    GCLK->GENDIV = objGeneratorDivision;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

    // Read GenCtrl
    *((uint8_t*)&(GCLK->GENCTRL)) = self->uGenericClockId;
    GCLK_GENCTRL_Type objGeneratorControl = GCLK->GENCTRL;

    bool             bGeneratorWasEnabled = objGeneratorControl.bit.GENEN;
    enum ClockSource eOldClockSource      = objGeneratorControl.bit.SRC;


    // Write to Genctrl
    self->objInputControl.bit.GENEN = 1;
    GCLK->GENCTRL = self->objInputControl;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

    // Disable old clock
    if (bGeneratorWasEnabled)
    {
        _gclk_clock_stop(eOldClockSource);
    }
}

void gclk_set_output(uint8_t uGenericClockId, enum ClockOutput eClockOutput)
{
    assert(uGenericClockId < GCLK_ID_NUM);
    assert(eClockOutput < _eClockOutputMax);

    gclk_disable_output(uGenericClockId, eClockOutput);

    // Set the Generic Clock Output
    GCLK_CLKCTRL_Type objClockControl = (GCLK_CLKCTRL_Type)
    {
        .bit.ID    = eClockOutput,
        .bit.GEN   = uGenericClockId,
        .bit.CLKEN = 1
    };
    GCLK->CLKCTRL = objClockControl;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}

void gclk_disable_output(uint8_t uGenericClockId, enum ClockOutput eClockOutput)
{
    assert(uGenericClockId < GCLK_ID_NUM);
    assert(eClockOutput < _eClockOutputMax);

    // Set the Generic Clock Output
    GCLK_CLKCTRL_Type objClockControl =
    {
        .bit.ID    = eClockOutput,
        .bit.CLKEN = 0
    };
    GCLK->CLKCTRL = objClockControl;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

    __disable_irq();
    do
    {
        *((uint8_t*)&GCLK->CLKCTRL) = eClockOutput;
    } while (0 != GCLK->CLKCTRL.bit.CLKEN);
    __enable_irq();
}

// User must check for 0 return
static unsigned _gclk_get_dfll_input_frequency()
{
    // Only GCLK[1] can be configured as source
    static const unsigned uDfllSourceGenerator = 1;
    Gclk_t* self = &g_objHandleArray[uDfllSourceGenerator];


    // GENDIV.DIV if DIVSEL is 0
    unsigned uDivider = self->uDivisionFactor;

    if (1 == self->objInputControl.bit.DIVSEL)
    {
        // 2^(GENDIV.DIV+1)
        uDivider = pow(2, self->uDivisionFactor + 1);
    }

    unsigned uFreqHz = clock_get_frequency(self->objInputControl.bit.SRC);

    if (0 == uDivider)
    {
        uDivider = 1;
    }
    return uFreqHz / uDivider;
}

static void _gclk_data_init(void)
{
    // TODO: Remove
    for (unsigned i = 0; i < _eClockOutputMax; ++i)
    {
        g_arrClockSourceRefCount[i] = 0;
    }

    for (unsigned i = 0; i < GCLK_ID_NUM; ++i)
    {
        Gclk_t* self = &g_objHandleArray[i];

        self->uDivisionFactor = 0; // Actual value is not needed
        self->uGenericClockId = i;

        // Read GenCtrl
        *((uint8_t*)&GCLK->GENCTRL) = i;
        self->objInputControl       = GCLK->GENCTRL;

        if (self->objInputControl.bit.GENEN)
        {
            ++g_arrClockSourceRefCount[self->objInputControl.bit.SRC];
        }
    }
}

void _gclk_clock_start(enum ClockSource eClockSource)
{
    ++g_arrClockSourceRefCount[eClockSource];
    switch (eClockSource)
    {
    case eOSC32K:
        clock_osc32k_start();
        break;

    case eXOSC32K:
        clock_xosc32k_start();
        break;

    case eDFLL48M:
    {
        unsigned uFreq = _gclk_get_dfll_input_frequency();
        clock_dfll48m_start(uFreq);
        break;
    }

    case eOSC8M:
        clock_osc8m_start();
        break;

    default:
        assert(0);
        break;
    }
}

void _gclk_clock_stop(enum ClockSource eClockSource)
{
    if (--g_arrClockSourceRefCount[eClockSource] != 0)
    {
        // This is a situation we want to catch, hopefully this never happens
        // Looks like there is a superfluous gclk_clock_stop
        assert(g_arrClockSourceRefCount[eClockSource] > 0);

        // Clock Source is still being used by someone else.
        return;
    }

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
