// Copyright 2021, Patrick Rohr

#include "common/list.hpp"
#include <gtest/gtest.h>

using namespace SAMD;

TEST(ListTest, TestPushBack)
{
    List<int, 10> list;

    list.PushBack(5);
    list.PushBack(1);

    ASSERT_EQ(list.Size(), 2);

    auto it = list.Begin();
    ASSERT_EQ(*it, 5);
    ASSERT_EQ(*(++it), 1);

    unsigned count = 0;
    for (const auto& item : list)
    {
        (void)item;
        ++count;
    }

    ASSERT_EQ(list.Size(), count);
}

TEST(ListTest, TestErase)
{
    List<int, 10> list;

    list.PushBack(7);
    list.PushBack(7);
    list.PushBack(3);
    list.PushBack(7);

    ASSERT_EQ(list.Size(), 4);

    for (auto it = list.Begin(); it != list.End(); it++)
    {
        if (*it == 3)
        {
            it = list.Erase(it);
        }
    }

    ASSERT_EQ(list.Size(), 3);

    for (const auto& value : list) { ASSERT_EQ(value, 7); }
}

TEST(ListTest, TestPushFront)
{
    List<unsigned, 10> list;

    list.PushBack(2);
    list.PushFront(1);
    list.PushBack(3);
    list.PushFront(0);

    ASSERT_EQ(list.Size(), 4);

    auto it = list.Begin();
    for (unsigned i = 0; i < list.Size(); ++i)
    {
        ASSERT_EQ(*it, i);
        ++it;
    }
}
