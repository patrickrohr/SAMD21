/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <gtest/gtest.h>

#include <hal_utils/io_port.hpp>
#include <cstdint>

using namespace SAMD;

struct Register
{
    uint16_t a;
    uint16_t b;

    bool operator==(const Register& rhs) const
    {
        return a == rhs.a && b == rhs.b;
    }
};

TEST(IoPortTest, ReadAndWrite)
{
    Register fakeRegister;
    fakeRegister.a = 1234;
    fakeRegister.b = 49999;

    Register fakeRegister2;
    fakeRegister2.a = 122;
    fakeRegister2.b = 6379;

    IoPortRead<Register> portRead(&fakeRegister);
    IoPortWrite<Register> portWrite(&fakeRegister);

    Register resultRegister = portRead.Read();
    EXPECT_EQ(fakeRegister, resultRegister);

    portWrite.Write(fakeRegister2);
    EXPECT_EQ(fakeRegister, fakeRegister2);
}

TEST(IoPortTest, ReadWrite)
{
    Register fakeRegister;
    fakeRegister.a = 93;
    fakeRegister.b = 56;

    Register fakeRegister2;
    fakeRegister2.a = 8885;
    fakeRegister2.b = 52851;

    IoPortRW<Register> portReadWrite(&fakeRegister);

    Register resultRegister = portReadWrite.Read();
    EXPECT_EQ(resultRegister, fakeRegister);

    portReadWrite.Write(fakeRegister2);
    EXPECT_EQ(fakeRegister, fakeRegister2);
}
