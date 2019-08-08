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

    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->Get()), fakeRegister);
    EXPECT_EQ(
        const_cast<FakeWideRegister&>(reg_FAKEWIDE->Get()), fakeRegisterWide);

    RegisterGuard<FakeRegister> tmp_FAKE(*reg_FAKE);
    RegisterGuard<FakeWideRegister> tmp_FAKEWIDE(*reg_FAKEWIDE);

    EXPECT_EQ(tmp_FAKE.Get(), fakeRegister);
    EXPECT_EQ(tmp_FAKEWIDE.Get(), fakeRegisterWide);
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

    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->Get()), FakeRegister());
    EXPECT_EQ(
        const_cast<FakeWideRegister&>(reg_FAKEWIDE->Get()), FakeWideRegister());

    const_cast<FakeRegister&>(reg_FAKE->Get()) = CreateFakeRegister();
    const_cast<FakeWideRegister&>(reg_FAKEWIDE->Get()) =
        CreateFakeWideRegister();

    RegisterGuard<FakeRegister, false, Environment::eSimulation> tmp_FAKE(
        *reg_FAKE);
    RegisterGuard<FakeWideRegister, false, Environment::eSimulation>
        tmp_FAKEWIDE(*reg_FAKEWIDE);

    EXPECT_EQ(tmp_FAKE.Get(), fakeRegister);
    EXPECT_EQ(tmp_FAKEWIDE.Get(), fakeRegisterWide);
}

TEST(RegisterGuardTest, SharedSimTest)
{
    // For the simulation, the underlying data structure shouldn't matter
    FakeRegister fakeRegister = CreateFakeRegister();

    auto* reg_FAKE =
        MakeSharedRegisterGuard<FakeRegister, Environment::eSimulation>(
            &fakeRegister);

    // Write ID 15
    *reinterpret_cast<volatile uint8_t*>(reg_FAKE) = 15;
    auto tmp_FAKE = *reg_FAKE;
    tmp_FAKE.Get().a = 1;
    tmp_FAKE.Get().b = 2;
    tmp_FAKE.Get().c = 37;

    *reg_FAKE = tmp_FAKE;

    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->Get()), tmp_FAKE.Get());

    // Write ID 20
    *reinterpret_cast<volatile uint8_t*>(reg_FAKE) = 20;
    auto tmp_FAKE2 = *reg_FAKE;
    tmp_FAKE2.Get().b = 46;

    *reg_FAKE = tmp_FAKE2;

    // ID Field of Shared register is equivalent to the a member here
    *reinterpret_cast<volatile uint8_t*>(reg_FAKE) = static_cast<uint8_t>(15);
    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->Get()).a, 15u);
    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->Get()).c, 37u);

    *reinterpret_cast<volatile uint8_t*>(reg_FAKE) = static_cast<uint8_t>(20);
    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->Get()).a, 20u);
    EXPECT_EQ(const_cast<FakeRegister&>(reg_FAKE->Get()).b, 46u);
}
