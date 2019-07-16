/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <cstdint>

namespace SAMD
{

template<unsigned SIZE>
struct fixed_width_int
{
    using type                     = char;
    static constexpr unsigned size = SIZE;
};

template<>
struct fixed_width_int<1>
{
    using type                     = uint8_t;
    static constexpr unsigned size = 1;
};

template<>
struct fixed_width_int<2>
{
    using type                     = uint16_t;
    static constexpr unsigned size = 2;
};

template<>
struct fixed_width_int<4>
{
    using type                     = uint32_t;
    static constexpr unsigned size = 4;
};

template<>
struct fixed_width_int<8>
{
    using type                     = uint64_t;
    static constexpr unsigned size = 8;
};

} // namespace SAMD
