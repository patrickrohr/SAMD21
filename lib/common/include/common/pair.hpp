/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

namespace SAMD {

template<typename T, typename U>
class Pair
{
public:
    Pair(const T& first_, const U& second_) :
        first(first_),
        second(second_)
    {
    }

public:
    T first;
    U second;
};

}
