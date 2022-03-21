/**
 * @file        test_serializing.cpp
 * @brief       Tests serializing of bitfields.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfield.hpp"

using namespace jungles;

enum class Reg
{
    field1,
    field2,
    field3,
    field4,
    field5,
    field6,
    field7,
    field8
};

TEST_CASE("One byte-long bitfield group is serialized", "[serializing]")
{
    Bitfield<uint8_t, ByteOrder::big, Field{Reg::field1, 2}, Field{Reg::field2, 4}, Field{Reg::field3, 2}> bf;

    bf.at<Reg::field1>() = 0b10;
    bf.at<Reg::field2>() = 0b0110;
    bf.at<Reg::field3>() = 0b01;

    REQUIRE(bf.serialize() == 0b10011001);
}
