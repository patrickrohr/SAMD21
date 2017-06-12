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
	ERROR_LEVEL_FATAL, // placeholder until I have some actual errors. Please never ever use this. Thanks.
	ERROR_BUFFER_POOL_MEMORY_LEAK
};

// Singleton pattern
void error_set(enum error_codes error_code);
void error_clear();

#endif