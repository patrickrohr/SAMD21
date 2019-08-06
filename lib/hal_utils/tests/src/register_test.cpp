/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <gtest/gtest.h>

#include <array>
#include <hal_utils/register.hpp>

using namespace SAMD;

struct FakeRegister
{
    uint32_t a : 8;
    uint32_t b : 8;
    uint32_t c : 16;

    FakeRegister() : a(), b(), c()
    {
    }

    bool operator==(const volatile FakeRegister& rhs) const volatile
    {
        return a == rhs.a && b == rhs.b && c == rhs.c;
    }
};

struct FakeWideRegister
{
    std::array<uint8_t, 16> data;

    FakeWideRegister() : data()
    {
    }

    FakeWideRegister(std::array<uint8_t, 16>&& arrData) :
        data(std::move(arrData))
    {
    }

    bool operator==(const FakeWideRegister& rhs) const
    {
        return data == rhs.data;
    }
};

FakeRegister CreateFakeRegister()
{
    FakeRegister reg;
    reg.a = 235;
    reg.b = 12;
    reg.c = 9649;

    return reg;
}

FakeWideRegister CreateFakeWideRegister()
{
    FakeWideRegister reg(
        { 123, 90, 23, 76, 23, 45, 1, 237, 11, 90, 72, 183, 3, 2, 2, 255 });

    return reg;
}

TEST(RegisterGuardTest, TargetTest)
{
    FakeRegister fakeRegister         = CreateFakeRegister();
    FakeWideRegister fakeRegisterWide = CreateFakeWideRegister();

    auto* reg_FAKE =
        MakeRegisterGuard<FakeRegister, Environment::eTarget>(&fakeRegister);
    auto* reg_FAKEWIDE =
        MakeRegisterGuard<FakeWideRegister, Environment::eTarget>(
            &fakeRegisterWide);

    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->data), fakeRegister);
    EXPECT_EQ(
        const_cast<FakeWideRegister&>(reg_FAKEWIDE->data), fakeRegisterWide);

    RegisterGuard<FakeRegister> tmp_FAKE(*reg_FAKE);
    RegisterGuard<FakeWideRegister> tmp_FAKEWIDE(*reg_FAKEWIDE);

    EXPECT_EQ(tmp_FAKE.data, fakeRegister);
    EXPECT_EQ(tmp_FAKEWIDE.data, fakeRegisterWide);
}

TEST(RegisterGuardTest, SimTest)
{
    FakeRegister fakeRegister         = CreateFakeRegister();
    FakeWideRegister fakeRegisterWide = CreateFakeWideRegister();

    auto* reg_FAKE = MakeRegisterGuard<FakeRegister, Environment::eSimulation>(
        &fakeRegister);
    auto* reg_FAKEWIDE =
        MakeRegisterGuard<FakeWideRegister, Environment::eSimulation>(
            &fakeRegisterWide);

    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->data), FakeRegister());
    EXPECT_EQ(
        const_cast<FakeWideRegister&>(reg_FAKEWIDE->data), FakeWideRegister());

    const_cast<FakeRegister&>(reg_FAKE->data) = CreateFakeRegister();
    const_cast<FakeWideRegister&>(reg_FAKEWIDE->data) =
        CreateFakeWideRegister();

    RegisterGuard<FakeRegister> tmp_FAKE(*reg_FAKE);
    RegisterGuard<FakeWideRegister> tmp_FAKEWIDE(*reg_FAKEWIDE);

    EXPECT_EQ(tmp_FAKE.data, fakeRegister);
    EXPECT_EQ(tmp_FAKEWIDE.data, fakeRegisterWide);
}
