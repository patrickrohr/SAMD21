
/*
* SAMD21G18.h
*
* Created: 6/14/2017 7:30:41 PM
*  Author: Patrick Rohr
*/ 


#include <stdint.h>

/* 
SERCOM Definition
*/


struct CTRLA_register
{
	// 0:7
	volatile uint8_t SWRST : 1;
	volatile uint8_t ENABLE : 1;
	volatile uint8_t MODE : 3;
	uint8_t : 2;
	volatile uint8_t RUNSTDBY : 1;

	// 8:15
	volatile uint8_t IBON : 1;
	uint8_t : 4;
	volatile uint8_t SAMPR : 3;

	// 16:23
	volatile uint8_t TXPO : 2;
	uint8_t : 2;
	volatile uint8_t RXPO : 2;
	volatile uint8_t SAMPA : 2;

	// 24:31
	volatile uint8_t FORM : 4;
	volatile uint8_t CMODE : 1;
	volatile uint8_t CPOL : 1;
	volatile uint8_t DORD : 1;
	uint8_t : 1;
};

struct CTRLB_register
{
	// 0:7
	volatile uint8_t CHSIZE : 3;
	uint8_t : 3;
	volatile uint8_t SBMODE : 1;
	uint8_t : 1;

	// 8:15
	volatile uint8_t COLDEN : 1;
	volatile uint8_t SFDE : 1;
	volatile uint8_t ENC : 1;
	uint8_t : 2;
	volatile uint8_t PMODE : 2;
	uint8_t : 2;

	// 16:23
	volatile uint8_t TXEN : 1;
	volatile uint8_t RXEN : 1;
	uint8_t : 6;

	// 24:31
	uint8_t : 8;
};

struct INT_register
{
	volatile uint8_t DRE : 1;
	volatile uint8_t TXC : 1;
	volatile uint8_t RXC : 1;
	volatile uint8_t RXS : 1;
	volatile uint8_t CTSIC : 1;
	volatile uint8_t RXBRK : 1;
	uint8_t : 1;
	volatile uint8_t ERROR : 1;
};

struct STATUS_register
{
	// 0:7
	volatile uint8_t PERR : 1;
	volatile uint8_t FERR : 1;
	volatile uint8_t BUFOVF : 1;
	volatile uint8_t CTS : 1;
	volatile uint8_t ISF : 1;
	volatile uint8_t COLL : 1;
	uint8_t : 2;

	// 8:15
	uint8_t : 8;
};

struct SYNCBUSY_register
{
	// 0:7
	volatile uint8_t SWRST : 1;
	volatile uint8_t ENABLE : 1;
	volatile uint8_t CTRLB : 1;
	uint8_t : 5;
	
	// 8:31
	uint8_t : 8;
	uint16_t : 16;
};

struct DATA_register
{
	volatile uint16_t DATA : 9;
	uint16_t : 7;
};

struct SERCOM {
	struct CTRLA_register CTRLA;
	struct CTRLB_register CTRLB;
	uint32_t : 32; //reserved
	volatile uint16_t BAUD;
	volatile uint8_t RXPL;
	uint8_t : 8; //reserved
	uint32_t : 32; //reserved
	struct INT_register INTENCLR;
	uint8_t : 8; //reserved
	struct INT_register INTENSET;
	uint8_t : 8; //reserved
	struct INT_register INTFLAG;
	uint8_t : 8; //reserved
	struct STATUS_register STATUS;
	struct SYNCBUSY_register SYNCBUSY;
	uint32_t : 32; //reserved
	uint32_t : 32; //reserved
	struct DATA_register DATA;
	uint16_t : 16; //reserved
	uint32_t : 32; //reserved
	//uint8_t DBGCTRL;
};


#define SERCOM0 ((struct SERCOM *)0x42000800UL)
#define SERCOM2 ((struct SERCOM *)0x42001000UL)
#define SERCOM4 ((struct SERCOM *)0x42001800UL)