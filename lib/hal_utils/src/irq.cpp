/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "hal_utils/irq.hpp"
#include "common/error.hpp"

extern void __disable_irq();

namespace SAMD
{

template<Environment ENV>
int InterruptSafeGuardImpl<ENV>::m_depthCount = 0;

} // namespace SAMD
