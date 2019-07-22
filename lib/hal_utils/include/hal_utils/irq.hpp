/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <common/environment.hpp>
#include <common/error.hpp>

namespace SAMD
{

template<Environment ENV = eRuntimeEnvironment>
class InterruptSafeGuardImpl
{
public:
    InterruptSafeGuardImpl();

    ~InterruptSafeGuardImpl();

private:
    static int g_depthCount;
};

using InterruptSafeGuard = InterruptSafeGuardImpl<>;

} // namespace SAMD
