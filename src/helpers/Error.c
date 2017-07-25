/*
* Error.c
*
* Created: 6/8/2017 7:58:27 AM
*  Author: Patrick Rohr
*/ 


#include "Error.h"


// PRIVATE

struct _Error
{
  uint16_t last_timestamp;
  uint16_t error_bits;
};


// PUBLIC

struct _Error * _error_get_instance()
{
  static struct _Error error_obj = {.error_bits = 0, .last_timestamp = 0};
  return &error_obj;
}

void error_set(enum error_codes error_code)
{
  _error_get_instance()->error_bits &= (1<<error_code);
}

void error_clear()
{
  _error_get_instance()->error_bits = 0;
}
