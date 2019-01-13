#include <samd21.h>
#include <gclk.h>

int main()
{
    // Set Flash Wait States to 1 for 3.3V operation
    NVMCTRL->CTRLB.bit.RWS = 1;

    // Turn on the digital interface clock
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

    // Set Up DFLL
    gclk_init();

    gclk_configure(1, eOSC32K);
    gclk_enable_generator(1);
    gclk_enable_peripheral_channel(1, eGCLK_DFLL48M_REF);

    // TODO: should configure and enable really be two steps
    gclk_configure(0, eDFLL48M);
    gclk_enable_generator(0);

    unsigned uPinLed = 17;
    PORT->Group[0].DIRSET.reg = (1 << uPinLed);
    PORT->Group[0].OUTSET.reg = (1 << uPinLed);

    while (1)
    {
        asm ("nop");
    }
    return 0;
}
