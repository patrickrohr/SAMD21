// Copyright 2021, Patrick Rohr

#pragma once

#include "clock/samd/dfll48m.hpp"
#include "clock/samd/osc8m.hpp"
#include "clock/samd/xosc32k.hpp"
#include "clock/sim/sim_clock.hpp"
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
