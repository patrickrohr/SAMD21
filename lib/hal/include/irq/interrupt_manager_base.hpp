// Copyright 2021, Patrick Rohr

#pragma once

#include <common/id_traits.hpp>

namespace SAMD
{

enum class IrqType
{
    eInvalid,
    eSYSCTRL,
};

class InterruptManagerBase
{
public:
    enum class Result
    {
        eNotHandled,
        eHandled,
        eHandledContinue
    };

    using InterruptFunc = Result (*)(void*);

public:
    using index_t                   = id_traits<InterruptManagerBase, unsigned>;
    virtual ~InterruptManagerBase() = 0;

    virtual index_t RegisterInterruptHandler(
        IrqType eType,
        InterruptFunc handler,
        void* data)                                        = 0;
    virtual void DeregisterInterruptHandler(index_t index) = 0;
};

InterruptManagerBase::~InterruptManagerBase() = default;

} // namespace SAMD
