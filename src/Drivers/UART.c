/*
* UART.c
*
* Created: 6/12/2017 8:29:27 PM
*  Author: Patrick Rohr
*/ 

#include "UART.h"
#include "PortIO.h"
// PRIVATE

// waits for CTRLB, ENABLE, or SWRST sync to happen
void _uart_sync(Sercom * self)
{
	while(self->USART.SYNCBUSY.reg);
}

void _uart_port_clock_init(UART_t * self)
{
	/* Enable Peripheral Access to SERCOMs */
	PM->APBCMASK.reg |= PM_APBCMASK_PAC2;

	/* Enable SERCOM in PM */
	switch((int)self)
	{
		case (int)SERCOM0: // it works, but I HATE this switch case statement.... FIXIT, TODO
			PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;
			break;
	}
}

void _uart_peripheral_clock_init(UART_t * self)
{
	// configure sercom clock to run off generic clock source 7 (DFLL48M)
	// generic clock generator 0 (system main clock) uses 48MHz DFLL
	switch((int)self)
	{
		case (int)SERCOM0:
			GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE) | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0x0); // must be single 16-bit write
			break;
	}
	
}

// TODO: refactor this... (take all the pins out of here, too tired right now to do it. good night)
void _uart_init(Sercom * self)
{
	// Reset UART
	self->USART.CTRLA.bit.SWRST = 1;
	_uart_sync(self);

	// Configure UART
	self->USART.CTRLA.bit.MODE = 0x1;
	self->USART.CTRLA.bit.CMODE = 0x0;
	self->USART.CTRLA.bit.RXPO = 0x3; // 0x3 = pad 3
	self->USART.CTRLA.bit.TXPO = 0x1; // 0x1 = pad 2 - this is not a mistake!!
	_uart_sync(self);
	self->USART.CTRLB.bit.CHSIZE = 0x0;
	_uart_sync(self);
	self->USART.CTRLA.bit.DORD = 0x0;
	self->USART.CTRLA.bit.FORM = 0x0;
	_uart_sync(self);
	self->USART.CTRLB.bit.SBMODE = 0x0;
	_uart_sync(self);

	// Calculate baud rate	
	uint64_t baudrate = 9600;
	self->USART.BAUD.reg = (uint16_t) (65535 - (65535 * (16 * (baudrate))) / SystemCoreClock);
	_uart_sync(self);

	self->USART.CTRLB.bit.RXEN = 1;
	self->USART.CTRLB.bit.TXEN = 1;
	_uart_sync(self);


	self->USART.INTENSET.bit.TXC = 1; // Transmitter interrupt enabled
	_uart_sync(self);

	// FINALLY! Enable UART
	// must be called very last, otherwise registers are read only
	self->USART.CTRLA.bit.ENABLE = 1;
	_uart_sync(self);
}

//PUBLIC

//struct SERCOM * uart_create(struct SERCOM * uart_addr)
// right now, this is somewhat singleton "by design" (or accident, *cough*). I should change this... Yet another TODO!
UART_t * uart_create(UART_t * serial_port, int pin_rx, int pin_tx)
{
	// pin rx: 11, pin tx: 10
	// initialize and turn on port
	_uart_port_clock_init(serial_port);

	// now we configure pins for uart use
	PortIO_t * port = port_io_create(PORT_A);
	port_io_uart_init(port, pin_rx, pin_tx);

	// now configure peripheral clock...
	_uart_peripheral_clock_init(serial_port);
	
	// initialize new UART
	_uart_init(serial_port);
	// if I would ever want to dynamically configure this, I could take the enable function out and allow to overwrite the uart object before enabling it. I could provide some functions for it...
	// TODO: see above.
	return serial_port;
}

void uart_send(UART_t * self, char data)
{
	self->USART.DATA.reg = 0x5A; // char to int. Looks weird, according to the internet that's fine. I'm a millennial, so internet == truth.
}