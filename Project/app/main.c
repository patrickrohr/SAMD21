#include <samd21.h>
#include <gclk.h>

int main()
{
    // SystemInitMain();
    // board_init();

    NVMCTRL->CTRLB.bit.RWS = 1;         // Set Flash Wait States to 1 for 3.3V operation

    /* Turn on the digital interface clock */
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

    Gclk_t objGclk;
    gclk_init(&objGclk, 0, eDFLL48M);
    gclk_enable_generator(&objGclk);

    unsigned uPinLed = 17;
    PORT->Group[0].DIRSET.reg = (1 << uPinLed);
    PORT->Group[0].OUTSET.reg = (1 << uPinLed);

    while (1)
    {
        asm ("nop");
    }
    return 0;
}
