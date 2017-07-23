/*
* UART.c
*
* Created: 6/12/2017 8:29:27 PM
*  Author: Patrick Rohr
*/ 

#include "UART.h"
#include "PortIO.h"
#include "Error.h"
#include "BufferPool.h"
#include "PacketType.h"
#include "FreeRTOS.h"
#include <stdbool.h>
#include <string.h>

#define RES 0xFF

// PRIVATE
/* Hidden UART Handle */
//TODO: be consistent with ptr naming
struct _UART {
	SercomUsart * port;
	Packet_t * packet_rx;
	QueueHandle_t q_rx;
	Packet_t * packet_tx;
	void * packet_rx_ptr;
	uint16_t packet_rx_length;
	uint16_t pin_rx;
	uint16_t pin_tx;
	uint16_t number;
	uint32_t status;
	SemaphoreHandle_t semaphore_rx;
};


uint8_t _sercom_pads[32] = {
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
	while(self->port->SYNCBUSY.reg);
}

void _uart_port_clock_init(UART_t * self)
{
	/* Enable Peripheral Access to SERCOMs */
	PM->APBCMASK.reg |= PM_APBCMASK_PAC2;

	/* Enable SERCOM in PM */
	switch(self->number)
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
	switch(self->number)
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
	self->port->CTRLA.bit.SWRST = 1;
	_uart_sync(self);

	// Configure UART
	self->port->CTRLA.bit.MODE = 0x1;
	self->port->CTRLA.bit.CMODE = 0x0;
	self->port->CTRLA.bit.RXPO = _uart_get_rx_pad(self->pin_rx); // 0x3 = pad 3
	self->port->CTRLA.bit.TXPO = _uart_get_tx_pad(self->pin_tx); // 0x1 = pad 2 - this is not a mistake!!
	_uart_sync(self);
	self->port->CTRLB.bit.CHSIZE = 0x0;
	_uart_sync(self);
	self->port->CTRLA.bit.DORD = 0x1;
	self->port->CTRLA.bit.FORM = 0x0;
	_uart_sync(self);
	self->port->CTRLB.bit.SBMODE = 0x0;
	_uart_sync(self);

	// Calculate baud rate	
	uint64_t baudrate = 19200;
	self->port->BAUD.reg = (uint16_t) (65535 - (65535 * (16 * (baudrate))) / SystemCoreClock);
	_uart_sync(self);

	self->port->CTRLB.bit.RXEN = 1;
	self->port->CTRLB.bit.TXEN = 1;
	_uart_sync(self);

	/* Enable RX Complete interrupt */
	self->port->INTENSET.reg = SERCOM_USART_INTENSET_RXC;
	_uart_sync(self);

	//self->port->INTENSET.bit.TXC = 1; // Transmitter interrupt enabled

	// FINALLY! Enable UART
	// must be called very last, otherwise registers are read only
	self->port->CTRLA.bit.ENABLE = 1;
	_uart_sync(self);

}


//PUBLIC
// effectively multiton
UART_t * uart_get_instance(UART_enum uart_number)
{
	// create uart instance array and initialize to null pointer
	static UART_t * uart_instances[5] = {0}; // I believe there are 5 uarts on the chip.

	// check if this uart has been initialized
	if(uart_instances[uart_number] == NULL)
	{
		uart_instances[uart_number] = pvPortMalloc(sizeof(UART_t));
		uart_instances[uart_number]->number = uart_number;
		uart_instances[uart_number]->port = (SercomUsart *)(SERCOM(uart_number));
		uart_instances[uart_number]->status = 0;
	}
	return uart_instances[uart_number];
}

bool uart_is_init(UART_t * self)
{
	return (self->status & (1 << UART_INITIALIZED));
}

void uart_init(UART_t * self, uint16_t pin_rx, uint16_t pin_tx, SemaphoreHandle_t semaphore_rx)
{
	self->pin_rx = pin_rx;
	self->pin_tx = pin_tx;
	self->semaphore_rx = semaphore_rx; // semaphore to be used between rx isr and task rx
	
	// pin rx: 11, pin tx: 10
	// initialize and turn on port
	_uart_port_clock_init(self);

	// now we configure pins for uart use
	PortIO_t * port = port_io_create(PORT_A);
	port_io_uart_init(port, self->pin_rx, self->pin_tx);

	// now configure peripheral clock...
	_uart_peripheral_clock_init(self);
	
	NVIC_EnableIRQ(SERCOM0_IRQn);

	// initialize new UART
	_uart_init(self);

	// if I would ever want to dynamically configure this, I could take the enable function out and allow to overwrite the uart object before enabling it. I could provide some functions for it...
	// TODO: see above.

	self->status |= (1 << UART_INITIALIZED);
}

void uart_send(UART_t * self, char data)
{
	// wait for empty data register
	while(!self->port->INTFLAG.bit.DRE); 
	self->port->DATA.reg = data;
}


void uart_send_string(UART_t * self, char * data)
{
	for(int i = 0; i < strlen(data); ++i)
	{
		uart_send(self, data[i]);
	}
}

void uart_send_packet(UART_t * self, Packet_t * packet)
{
	switch(packet->data_type)
	{
		case PACKET_TYPE_TEXT: ;
			TextPacket_t * text_packet = (TextPacket_t *) &(packet->data);
			
			// put in its own function to clean this up...
			for(int i = 0; i < text_packet->length; i++)
			{
				char letter =  *((char*)(&(text_packet->text) + i));
				uart_send(self, letter);
			}
			break;
	}
}

void uart_set_q_rx(UART_t * self, QueueHandle_t q_rx)
{
	self->q_rx = q_rx;
}

QueueHandle_t * uart_get_q_rx(UART_t * self)
{
	return self->q_rx;
}

void uart_set_packet_rx(UART_t * self, Packet_t * packet)
{
	self->packet_rx = packet;
}

Packet_t * uart_get_packet_rx(UART_t * self)
{
	return self->packet_rx;
}

SemaphoreHandle_t uart_get_semaphore_rx(UART_t * self)
{
	return self->semaphore_rx;
}