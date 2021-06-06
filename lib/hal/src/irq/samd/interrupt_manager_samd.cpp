// Copyright 2021, Patrick Rohr

#include "irq/samd/interrupt_manager_samd.hpp"

void SYSCTRL_Handler()
{
    auto& obj = SAMD::InterruptManagerSamd::GetInstance();
    obj.HandleInterrupt(SAMD::IrqType::eSYSCTRL);
}

namespace SAMD
{

InterruptManagerSamd& InterruptManagerSamd::GetInstance()
{
    static InterruptManagerSamd obj;
    return obj;
}

InterruptManagerBase::index_t InterruptManagerSamd::RegisterInterruptHandler(
    IrqType eType,
    InterruptFunc handler,
    void* data)
{
    if (!handler)
    {
        // TODO: error...
        return InterruptManagerBase::index_t(m_interrupts.Size());
    }

    for (unsigned i = 0; i < m_interrupts.Size(); ++i)
    {
        auto& obj = m_interrupts[i];
        if (obj.eType == IrqType::eInvalid)
        {
            obj.eType   = eType;
            obj.handler = handler;
            obj.data    = data;
            return InterruptManagerBase::index_t(i);
        }
    }
    // TODO: error, ran out of slots
    return InterruptManagerBase::index_t(m_interrupts.Size());
}

void InterruptManagerSamd::DeregisterInterruptHandler(
    InterruptManagerBase::index_t index)
{
    if (index.Get() >= m_interrupts.Size())
    {
        return;
    }

    auto& obj   = m_interrupts[index.Get()];
    obj.data    = nullptr;
    obj.handler = nullptr;
    obj.eType   = IrqType::eInvalid;
}

void InterruptManagerSamd::HandleInterrupt(IrqType eType)
{
    for (const auto& obj : m_interrupts)
    {
        if (obj.eType == eType)
        {
            // no need to check for nullptr as that is already done when the
            // handler is registered.
            auto result = obj.handler(obj.data);
            if (result == Result::eHandled)
            {
                // we only stop once the interrupt is "handled", and the handler
                // indicates that we should *not* continue.
                return;
            }
        }
    }
}

} // namespace SAMD
