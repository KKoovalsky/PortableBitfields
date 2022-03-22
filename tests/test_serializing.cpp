/**
 * @file        test_serializing.cpp
 * @brief       Tests serializing of bitfields.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfield.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("One-byte-long bitfield group is serialized", "[serializing]")
{
    Bitfield<uint8_t, ByteOrder::big, Field{Reg::field1, 2}, Field{Reg::field2, 4}, Field{Reg::field3, 2}> bf;

    bf.at<Reg::field1>() = 0b10;
    bf.at<Reg::field2>() = 0b0110;
    bf.at<Reg::field3>() = 0b01;

    REQUIRE(bf.serialize() == 0b10011001);
}

TEST_CASE("Two-byte-long bitfield group is serialized, big endian", "[serializing]")
{
    SECTION("Evenly aligned, four bitfields")
    {
        Bitfield<uint16_t,
                 ByteOrder::big,
                 Field{Reg::field1, 4},
                 Field{Reg::field2, 4},
                 Field{Reg::field3, 4},
                 Field{Reg::field4, 4}>
            bf;

        bf.at<Reg::field1>() = 0b0110;
        bf.at<Reg::field2>() = 0b1001;
        bf.at<Reg::field3>() = 0b1010;
        bf.at<Reg::field4>() = 0b0101;

        REQUIRE(bf.serialize() == 0b0110100110100101);
    }

    SECTION("Two small bitfields, and one big")
    {
        Bitfield<uint16_t, ByteOrder::big, Field{Reg::field1, 1}, Field{Reg::field2, 1}, Field{Reg::field3, 14}> bf;

        bf.at<Reg::field1>() = 0b1;
        bf.at<Reg::field2>() = 0b0;
        bf.at<Reg::field3>() = 0b00001111001100;

        REQUIRE(bf.serialize() == 0b1000001111001100);
    }
}

TEST_CASE("Four-byte-long bitfield group is serialized, big endian", "[serializing]")
{
    Bitfield<uint32_t, ByteOrder::big, Field{Reg::field1, 7}, Field{Reg::field2, 21}, Field{Reg::field3, 4}> bf;

    bf.at<Reg::field1>() = 0b1010101;
    bf.at<Reg::field2>() = 0b000011111111111110000;
    bf.at<Reg::field3>() = 0b0110;

    REQUIRE(bf.serialize() == 0b10101010000111111111111100000110);
}

TEST_CASE("Two-byte-long bitfield group is serialized, little endian", "[serializing]")
{
    SECTION("Evenly aligned, four bitfields")
    {
        Bitfield<uint16_t,
                 ByteOrder::little,
                 Field{Reg::field1, 4},
                 Field{Reg::field2, 4},
                 Field{Reg::field3, 4},
                 Field{Reg::field4, 4}>
            bf;

        bf.at<Reg::field1>() = 0b0110;
        bf.at<Reg::field2>() = 0b1001;
        bf.at<Reg::field3>() = 0b1010;
        bf.at<Reg::field4>() = 0b0101;

        REQUIRE(bf.serialize() == 0b1010010101101001);
    }

    SECTION("Two small bitfields, and one big")
    {
        Bitfield<uint16_t, ByteOrder::little, Field{Reg::field1, 1}, Field{Reg::field2, 1}, Field{Reg::field3, 14}> bf;

        bf.at<Reg::field1>() = 0b1;
        bf.at<Reg::field2>() = 0b0;
        bf.at<Reg::field3>() = 0b00001111001100;

        REQUIRE(bf.serialize() == 0b1100110010000011);
    }
}

TEST_CASE("Four-byte-long bitfield group is serialized, little endian", "[serializing]")
{
    Bitfield<uint32_t, ByteOrder::little, Field{Reg::field1, 7}, Field{Reg::field2, 21}, Field{Reg::field3, 4}> bf;

    bf.at<Reg::field1>() = 0b1010101;
    bf.at<Reg::field2>() = 0b000011111111111110000;
    bf.at<Reg::field3>() = 0b0110;

    REQUIRE(bf.serialize() == 0b00000110111111110001111110101010);
}
