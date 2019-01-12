#include <samd21.h>
#include <gclk.h>

int main()
{
    // Set Flash Wait States to 1 for 3.3V operation
    NVMCTRL->CTRLB.bit.RWS = 1;

    // Turn on the digital interface clock
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

    GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    while ((GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY));

    Gclk_t objGclk;
    gclk_init(&objGclk, 1, eOSC32K);
    gclk_enable_generator(&objGclk);
    gclk_enable_peripheral_channel(&objGclk, eGCLK_DFLL48M_REF);

    Gclk_t objGclkDFLL;
    gclk_init(&objGclkDFLL, 0, eDFLL48M);
    gclk_enable_generator(&objGclkDFLL);

    unsigned uPinLed = 17;
    PORT->Group[0].DIRSET.reg = (1 << uPinLed);
    PORT->Group[0].OUTSET.reg = (1 << uPinLed);

    while (1)
    {
        asm ("nop");
    }
    return 0;
}
