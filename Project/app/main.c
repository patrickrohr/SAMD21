#include <samd21.h>
#include <gclk.h>

int main()
{
    // Set Up DFLL
    gclk_init();
    gclk_enable_input(1, eXOSC32K);
    gclk_add_output(1, eGCLK_DFLL48M_REF);
    gclk_enable_input(0, eDFLL48M);

    unsigned uPinLed = 17;
    PORT->Group[0].DIRSET.reg = (1 << uPinLed);
    PORT->Group[0].OUTSET.reg = (1 << uPinLed);

    while (1)
    {
        asm ("nop");
    }
    return 0;
}
