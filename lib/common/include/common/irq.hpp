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

template<Environment ENV = eRuntimeEnvironment>
class InterruptSafeGuardImpl
{
public:
    InterruptSafeGuardImpl();

    ~InterruptSafeGuardImpl();

private:
    static int m_depthCount;
};

using InterruptSafeGuard = InterruptSafeGuardImpl<>;

} // namespace SAMD
