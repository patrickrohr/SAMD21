// Copyright 2021, Patrick Rohr

#pragma once

#include "common/array.hpp"
#include "irq/interrupt_manager_base.hpp"

namespace SAMD
{

class InterruptManagerSamd : public InterruptManagerBase
{
public:
    static InterruptManagerSamd& GetInstance();
    ~InterruptManagerSamd() override = default;

    index_t RegisterInterruptHandler(
        IrqType eType,
        InterruptFunc handler,
        void* data) override;
    void DeregisterInterruptHandler(index_t index) override;

    void HandleInterrupt(IrqType eType);

private:
    InterruptManagerSamd() = default;

    struct InterruptHolder
    {
        InterruptHolder() :
            data(nullptr), handler(nullptr), eType(IrqType::eInvalid)
        {
        }

        void* data;
        InterruptFunc handler;
        IrqType eType;
    };

    // Make this configurable
    Array<InterruptHolder, 30> m_interrupts;
};

} // namespace SAMD
