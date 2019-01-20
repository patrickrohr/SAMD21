#include <samd21.h>
#include <gclk.h>

int main()
{
    // Set Up DFLL
    gclk_init();
    gclk_set_input(1, eXOSC32K);
    gclk_enable(1);
    gclk_set_output(1, eGCLK_DFLL48M_REF);
    gclk_set_input(0, eDFLL48M);
    gclk_enable(0);

    unsigned uPinLed = 17;
    PORT->Group[0].DIRSET.reg = (1 << uPinLed);
    PORT->Group[0].OUTSET.reg = (1 << uPinLed);

    while (1)
    {
        asm ("nop");
    }
    return 0;
}
