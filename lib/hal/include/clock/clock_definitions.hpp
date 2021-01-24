/**************************************************************
 *                                                            *
 *               Copyright (c) 2021 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/impl/dfll48m.hpp"
#include "clock/impl/osc8m.hpp"
#include "clock/impl/sim_clock.hpp"
#include "clock/impl/xosc32k.hpp"
#include "cmakeconfig.h"

namespace SAMD
{

#ifdef BUILD_TARGET

using DfllClock      = DFLL48M;
using FastClock      = OSC8M;
using ReferenceClock = XOSC32K;

#else

using DfllClock      = SimClock;
using FastClock      = SimClock;
using ReferenceClock = SimClock;

#endif

} // namespace SAMD
