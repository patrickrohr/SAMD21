#include <cmsis.h>
#include <samd21.h>

// Copy paste from: https://community.atmel.com/forum/samd21-xplained-pro-operating-frequency
// Just so I can get this working and verify that my setup works.
// Turns out, this is working!!!!!
void board_init(void)
{
    // SYSCTRL->OSC8M.bit.PRESC = 0;       // Set OSC8M prescaler to 1

    NVMCTRL->CTRLB.bit.RWS = 1;         // Set Flash Wait States to 1 for 3.3V operation

    SYSCTRL->DFLLCTRL.reg = 0;          // See Errata 9905
    while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0);    // Wait for DFLL synchronization complete
    
    // DFLL default is open loop mode:
    SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL (48000);     // Set to multiply USB SOF frequency (when USB attached)

    // Set DFLL for USB Clock Recovery Mode, Bypass Coarse Lock, Disable Chill Cycle,
    //   Fine calibration register locks (stable) after fine lock
    SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE | SYSCTRL_DFLLCTRL_USBCRM |
                            SYSCTRL_DFLLCTRL_BPLCKC | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_STABLE;
    while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0);    // Wait for DFLL synchronization complete

    GCLK->GENDIV.reg = GCLK_GENDIV_ID (0) | GCLK_GENDIV_DIV (1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID (0) | GCLK_GENCTRL_SRC (GCLK_SOURCE_DFLL48M) |
                        GCLK_GENCTRL_RUNSTDBY | GCLK_GENCTRL_GENEN;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);    // Wait till synchronization is complete
}

int main()
{
    // SystemInitMain();
    board_init();

    unsigned uPinLed = 17;
    PORT->Group[0].DIRSET.reg = (1 << uPinLed);
    PORT->Group[0].OUTSET.reg = (1 << uPinLed);

    while(1)
    {
        asm ("nop");
    }
    return 0;
}
