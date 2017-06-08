/*
* Error.h
*
* Created: 6/8/2017 7:58:37 AM
*  Author: Patrick Rohr
*/

#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdint.h>
	
enum error_codes
{
	GENERIC_ERROR // placeholder until I have some actual errors. Please never ever use this. Thanks.
};

struct Error
{
	uint16_t last_timestamp;
	uint16_t error_bits;
};

struct Error * error_get_instance();
void error_set(struct Error * this, enum error_codes error_code);
void error_clear(struct Error * this);

#endif