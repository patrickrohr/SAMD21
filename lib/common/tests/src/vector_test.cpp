/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <gtest/gtest.h>
#include "common/vector.hpp"

using namespace SAMD;

TEST(VectorTest, Test)
{
    Vector<int, 3> vec;

    vec.PushBack(1);
    vec.PushBack(15);

    ASSERT_EQ(vec.Size(), 2);

    ASSERT_EQ(vec[0], 1);
    ASSERT_EQ(vec[1], 15);

    unsigned count = 0;
    for (const auto& item : vec)
    {
        (void) item;
        ++count;
    }

    ASSERT_EQ(vec.Size(), count);
}
