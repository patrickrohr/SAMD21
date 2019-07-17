/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "cmakeconfig.h"

namespace SAMD
{

enum class Environment
{
    eTarget,
    eSimulation
};

enum class Configuration
{
    eDebug,
    eRelease
};

#if BUILD_SIMULATION
static constexpr Environment eRuntimeEnvironment     = Environment::eSimulation;
static constexpr Configuration eRuntimeConfiguration = Configuration::eDebug;
#else
static constexpr Environment eRuntimeEnvironment     = Environment::eTarget;
static constexpr Configuration eRuntimeConfiguration = Configuration::eDebug;
#endif
} // namespace SAMD
