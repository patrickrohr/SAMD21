// Copyright 2019, Patrick Rohr

#pragma once

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

static constexpr Environment eRuntimeEnvironment     = Environment::eTarget;
static constexpr Configuration eRuntimeConfiguration = Configuration::eDebug;

} // namespace SAMD
