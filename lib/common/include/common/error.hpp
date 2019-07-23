/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "common/environment.hpp"

#if BUILD_SIMULATION
#include "cstdout"

#define samd_assert(statement, message, ...)  \
    do                                        \
    {                                         \
        if (!(statement))                     \
            printf((message), ##__VA_ARGS__); \
    } while (0)

#else

#define samd_assert(statement, message, ...) \
    do                                       \
    {                                        \
        while (!(statement)) {}              \
                                             \
    } while (0)

#endif

namespace SAMD
{

using error_t = int;

} // namespace SAMD
