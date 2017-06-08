/*
* Error.c
*
* Created: 6/8/2017 7:58:27 AM
*  Author: Patrick Rohr
*/ 


#include "Error.h"


struct Error * error_get_instance()
{
	static struct Error error_obj = {.error_bits = 0, .last_timestamp = 0};
	return &error_obj;
}

void error_set(struct Error * this, enum error_codes error_code)
{
	this->error_bits &= (1<<error_code);
}

void error_clear(struct Error * this)
{
	this->error_bits = 0;
}