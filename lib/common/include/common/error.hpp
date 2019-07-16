/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "common/environment.hpp"

namespace SAMD
{

using error_t = int;

template<Configuration CONFIG = eRuntimeConfiguration>
struct assert
{
    // add error code
    constexpr assert(bool statement)
    {
        // infinite while loop when assertion is false.
        while (!statement) {}
    }
};

template<>
struct assert<Configuration::eRelease>
{
    constexpr assert(bool statement)
    {
        (void)statement;
    }
};

} // namespace SAMD
