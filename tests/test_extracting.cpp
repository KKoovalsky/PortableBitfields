/**
 * @file        test_extracting.cpp
 * @brief       Tests extracting of bitfields.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfield.hpp"

#include <iostream>

using namespace jungles;

enum class Reg
{
    field1,
    field2,
    field3,
    field4
};

TEST_CASE("Extracts bitfields on one-byte long bitfield, for right-to-left field order", "[extract]")
{
    SECTION("For three fields, almost evenly aligned")
    {
        Bitfield<uint8_t,
                 ByteOrder::little,
                 FieldOrder::right_to_left,
                 Field{Reg::field1, 3},
                 Field{Reg::field2, 2},
                 Field{Reg::field3, 3}>
            bf;

        bf.at<Reg::field1>() = 0b101;
        bf.at<Reg::field2>() = 0b11;
        bf.at<Reg::field3>() = 0b010;

        REQUIRE(bf.extract<Reg::field1>() == 0b10100000);
        REQUIRE(bf.extract<Reg::field2>() == 0b00011000);
        REQUIRE(bf.extract<Reg::field3>() == 0b00000010);
    }

    SECTION("For four fields, with various sizes")
    {
        // 4, 2, 1, 1
    }

    SECTION("For each field having size one")
    {
    }

    SECTION("For one field, occupying the whole bitfield")
    {
    }
}

TEST_CASE("Extracts bitfields on one-byte long bitfield, for left-to-right field order", "[extract]")
{
}

TEST_CASE("Extracts bitfields on 4-byte long bitfield, for right-to-left field order, little endian", "[extract]")
{
}

TEST_CASE("Extracts bitfields on 4-byte long bitfield, for right-to-left field order, big endian", "[extract]")
{
}

TEST_CASE("Extracts bitfields on 4-byte long bitfield, for left-to-right field order, little endian", "[extract]")
{
}

TEST_CASE("Extracts bitfields on 4-byte long bitfield, for left-to-right field order, big endian", "[extract]")
{
}
