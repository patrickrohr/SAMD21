/*
* UART.c
*
* Created: 6/12/2017 8:29:27 PM
*  Author: Patrick Rohr
*/ 

#include "UART.h"
#include "PortIO.h"
#include "Error.h"

#define RES 0xFF

// PRIVATE
int _sercom_pads[32] = {
	RES, RES, RES, RES, RES, RES, RES, RES,
	0, 1, 2, 3, 0, 1, 2, 3,
	0, 1, 2, 3, 2, 3, 0, 1,
	2, 3, RES, RES, RES, RES, RES, RES
};

int _uart_get_tx_pad(int pin_tx)
{
	switch (_sercom_pads[pin_tx])
	{
		case 0:
			return 0x0;
		case 2:
			return 0x1; // no mistake... see manual.
	}
	// EXCEPTION
	error_set(ERROR_UART_PAD_MISMATCH_TX);
	return RES;
}

int _uart_get_rx_pad(int pin_rx)
{
	int pad = _sercom_pads[pin_rx];
	if (pad == RES)
	{
		error_set(ERROR_UART_PAD_MISMATCH_RX);
	}
	return pad;
}


// waits for CTRLB, ENABLE, or SWRST sync to happen
void _uart_sync(UART_t * self)
{
	while(self->UART->SYNCBUSY.reg);
}

void _uart_port_clock_init(UART_t * self)
{
	/* Enable Peripheral Access to SERCOMs */
	PM->APBCMASK.reg |= PM_APBCMASK_PAC2;

	/* Enable SERCOM in PM */
	switch(self->uart_number)
	{
		case UART0: // it works, but I HATE this switch case statement.... FIXIT, TODO
			PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;
			break;
	}
}

void _uart_peripheral_clock_init(UART_t * self)
{
	// configure sercom clock to run off generic clock source 7 (DFLL48M)
	// generic clock generator 0 (system main clock) uses 48MHz DFLL
	switch(self->uart_number)
	{
		case UART0:
			GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE) | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0x0); // must be single 16-bit write
			break;
	}
	
}

// TODO: refactor this... (take all the pins out of here, too tired right now to do it. good night)
void _uart_init(UART_t * self)
{
	// Reset UART
	self->UART->CTRLA.bit.SWRST = 1;
	_uart_sync(self);

	// Configure UART
	self->UART->CTRLA.bit.MODE = 0x1;
	self->UART->CTRLA.bit.CMODE = 0x0;
	self->UART->CTRLA.bit.RXPO = _uart_get_rx_pad(self->pin_rx); // 0x3 = pad 3
	self->UART->CTRLA.bit.TXPO = _uart_get_tx_pad(self->pin_tx); // 0x1 = pad 2 - this is not a mistake!!
	_uart_sync(self);
	self->UART->CTRLB.bit.CHSIZE = 0x0;
	_uart_sync(self);
	self->UART->CTRLA.bit.DORD = 0x0;
	self->UART->CTRLA.bit.FORM = 0x0;
	_uart_sync(self);
	self->UART->CTRLB.bit.SBMODE = 0x0;
	_uart_sync(self);

	// Calculate baud rate	
	uint64_t baudrate = 9600;
	self->UART->BAUD.reg = (uint16_t) (65535 - (65535 * (16 * (baudrate))) / SystemCoreClock);
	_uart_sync(self);

	self->UART->CTRLB.bit.RXEN = 1;
	self->UART->CTRLB.bit.TXEN = 1;
	_uart_sync(self);


	self->UART->INTENSET.bit.TXC = 1; // Transmitter interrupt enabled
	_uart_sync(self);

	// FINALLY! Enable UART
	// must be called very last, otherwise registers are read only
	self->UART->CTRLA.bit.ENABLE = 1;
	_uart_sync(self);
}

//PUBLIC
void uart_create(UART_t * self)
{
	// pin rx: 11, pin tx: 10
	// initialize and turn on port
	_uart_port_clock_init(self);

	// now we configure pins for uart use
	PortIO_t * port = port_io_create(PORT_A);
	port_io_uart_init(port, self->pin_rx, self->pin_tx);

	// now configure peripheral clock...
	_uart_peripheral_clock_init(self);
	
	// initialize new UART
	_uart_init(self);

	// if I would ever want to dynamically configure this, I could take the enable function out and allow to overwrite the uart object before enabling it. I could provide some functions for it...
	// TODO: see above.
}

void uart_send(UART_t * self, char data)
{
	self->UART->DATA.reg = 0x5A; // char to int. Looks weird, according to the internet that's fine. I'm a millennial, so internet == truth.
}

UART_t * uart_populate_handle(UART_t * uart, UARTn_enum uart_number, uint16_t pin_rx, uint16_t pin_tx)
{
	switch(uart_number)
	{
		case UART0:
			uart->UART = &(SERCOM0->USART);
			break;
		default:
			error_set(ERROR_UART_PORT_NOT_CONFIGURABLE);		
	}
	uart->pin_rx = pin_rx;
	uart->pin_tx = pin_tx;
	uart->uart_number = uart_number;
	return uart;
}