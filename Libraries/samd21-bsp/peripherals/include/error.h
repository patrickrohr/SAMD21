/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

// TODO: Implement assert
// The dream is for this function to provide some kind of debug information in some blocked off memory region
#define assert(value)    do { \
        while (!value);       \
} while (0)

#define E_ERROR              1
#define E_BAD_PARAM          2

#define E_NOT_IMPLEMENTED    16
