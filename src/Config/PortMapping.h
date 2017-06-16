
/*
* PortMapping.h
*
* Created: 6/14/2017 7:30:25 PM
*  Author: Patrick Rohr
*/ 

#ifndef __PORT_MAPPING_H__
#define __PORT_MAPPING_H__
#include "SAMD21G18A.h"

//TODO
/*
It is my vision for this file to eventually define all the port mappings of
my particular setup and act as an abstraction layer towards SAMD21G18A.h
*/

#define SERCOM_PIN_0_1		SERCOM0
#define SERCOM_PIN_0_1_RX	0x3
#define SERCOM_PIN_0_1_TX	0x2

#endif