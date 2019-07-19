/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <gtest/gtest.h>

#include <array>
#include <common/io_port.hpp>

using namespace SAMD;

struct FakeRegister
{
    uint32_t a : 8;
    uint32_t b : 8;
    uint32_t c : 16;

    FakeRegister() : a(), b(), c()
    {
    }

    bool operator==(const FakeRegister& rhs) const
    {
        return a == rhs.a && b == rhs.b && c == rhs.c;
    }
};

struct FakeWideRegister
{
    std::array<char, 16> data;

    FakeWideRegister() : data()
    {
    }

    bool operator==(const FakeWideRegister& rhs) const
    {
        return data == rhs.data;
    }
};

TEST(RegisterTypeTest, Constructor)
{

    volatile int fakeRegister = 15;

    SAMD::register_t<int, Environment::eTarget> objRegister(&fakeRegister);
    SAMD::register_t<int, Environment::eSimulation> objRegisterSim(
        &fakeRegister);

    EXPECT_EQ(objRegister.Get(), &fakeRegister);
    EXPECT_NE(objRegisterSim.Get(), &fakeRegister);
}

TEST(RegisterTypeTest, NormalRegister)
{
    FakeRegister fakeRegister;

    SAMD::register_t<decltype(fakeRegister), Environment::eTarget> objRegister(
        &fakeRegister);
    SAMD::register_t<decltype(fakeRegister), Environment::eSimulation>
        objRegisterSim(&fakeRegister);

    FakeRegister registerVal;
    registerVal.a = 5;
    registerVal.b = 123;
    registerVal.c = 8721;

    objRegister = registerVal;
    EXPECT_EQ(fakeRegister, registerVal);

    objRegisterSim = registerVal;
    // HACK: Casting away volatile qualifier using const_cast
    EXPECT_EQ(*const_cast<FakeRegister*>(objRegisterSim.Get()), registerVal);

    FakeRegister result = objRegister;
    EXPECT_EQ(result, fakeRegister);
    EXPECT_EQ(result, registerVal);

    result = objRegisterSim;
    EXPECT_EQ(result, registerVal);
}

TEST(RegisterTypeTest, WideRegister)
{
    FakeWideRegister fakeRegister;

    SAMD::register_t<decltype(fakeRegister), Environment::eTarget> objRegister(
        &fakeRegister);
    SAMD::register_t<decltype(fakeRegister), Environment::eSimulation>
        objRegisterSim(&fakeRegister);

    FakeWideRegister registerVal;
    registerVal.data[0]  = 35;
    registerVal.data[1]  = 253;
    registerVal.data[2]  = 1;
    registerVal.data[3]  = 37;
    registerVal.data[7]  = 1;
    registerVal.data[11] = 17;
    registerVal.data[13] = 178;

    objRegister = registerVal;
    EXPECT_EQ(fakeRegister, registerVal);

    objRegisterSim = registerVal;
    // HACK: Casting away volatile qualifier using const_cast
    EXPECT_EQ(
        *const_cast<FakeWideRegister*>(objRegisterSim.Get()), registerVal);

    FakeWideRegister result = registerVal;
    EXPECT_EQ(result, fakeRegister);
    EXPECT_EQ(result, registerVal);

    result = objRegisterSim;
    EXPECT_EQ(result, registerVal);
}
