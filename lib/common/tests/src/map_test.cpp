/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <gtest/gtest.h>

#include "common/map.hpp"
#include <string>
#include <iostream>

using namespace SAMD;

TEST(MapTest, InsertTest)
{
    Map<int, std::string, 1> mapSmall;

    ASSERT_TRUE(mapSmall.Insert({ 5, "this is a test" }));
    ASSERT_FALSE(mapSmall.Insert({ 3, "this is another test" }));

    Map<int, std::string, 127> mapLarge;

    ASSERT_TRUE(mapLarge.Insert({ 13, "root" }));
    ASSERT_TRUE(mapLarge.Insert({ 10, "left" }));
    ASSERT_TRUE(mapLarge.Insert({ 15, "right" }));
    ASSERT_TRUE(mapLarge.Insert({ 9, "left - left" }));
    ASSERT_TRUE(mapLarge.Insert({ 3, "left - left - left" }));
    ASSERT_FALSE(mapLarge.Insert({ 3, "left - left - left" }));
}

TEST(MapTest, FindTest)
{
    Map<int, std::string, 127> map;

    ASSERT_TRUE(map.Insert({ 12, "12" }));
    ASSERT_TRUE(map.Insert({ 19, "19" }));
    ASSERT_TRUE(map.Insert({ 2, "2" }));
    ASSERT_TRUE(map.Insert({ 1, "1" }));
    ASSERT_TRUE(map.Insert({ 17, "17" }));
    ASSERT_TRUE(map.Insert({ 7, "7" }));
    ASSERT_TRUE(map.Insert({ 77, "77" }));

    auto itFound = map.Find(12);
    ASSERT_NE(itFound, map.End());
    ASSERT_EQ(itFound->value, "12");

    itFound = map.Find(7);
    ASSERT_NE(itFound, map.End());
    ASSERT_EQ(itFound->value, "7");

    itFound = map.Find(-12);
    ASSERT_EQ(itFound, map.End());

    ASSERT_EQ(7u, map.Size());

    int lastKey = map.Begin()->key;
    for (const auto& item : map)
    {
        ASSERT_GE(item.key, lastKey);
        lastKey = item.key;
    }
}
