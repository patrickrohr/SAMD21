/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "serial/sercom_base.hpp"

namespace SAMD
{

struct UsartConfiguration
{
    enum class Mode
    {
        eExternalClk = static_cast<unsigned>(SercomBase::Mode::eSercomUsartExternalClk),
        eInternalClk = static_cast<unsigned>(SercomBase::Mode::eSercomUsartInternalClk)
    };

    enum class CommMode
    {
        eAsync,
        eSync
    };

    enum class RxPad
    {
        eTODO,
    };

    enum class DataOrder
    {
        eMsbFirst,
        eLsbFirst,
    };

    enum class CharacterSize
    {
        eTODO
    };

    // Add Configuration options here
    static constexpr Mode Mode = Mode::eExternalClk;
    static constexpr CommMode CommMode = CommMode::eAsync;
    static constexpr RxPad RxPad = RxPad::eTODO;
    static constexpr DataOrder DataOrder = DataOrder::eMsbFirst;
    static constexpr CharacterSize CharacterSize = CharacterSize::eTODO;
};

template<typename CONFIG = UsartConfiguration>
class Usart : public SercomBase
{
public:
    // I like the idea of compile time configuration here,
    // so really, this could support RAII. Init will be called from constructor.
    Usart(sercom_id_t id, ClockSourceGeneric& sourceClock);
    ~Usart() override;

private:
    void Init();
    void Enable();
    void Disable();
};

} // namespace SAMD
