/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <gtest/gtest.h>
#include "common/array.hpp"

using namespace SAMD;

TEST(ArrayTest, Test)
{
    Array<int, 3> arr;

    arr[0] = 1;
    arr[1] = 15;
    arr[2] = 17;

    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[1], 15);
    ASSERT_EQ(arr[2], 17);

    unsigned count = 0;
    for (const auto& item : arr)
    {
        (void) item;
        ++count;
    }

    ASSERT_EQ(arr.Size(), count);
}
