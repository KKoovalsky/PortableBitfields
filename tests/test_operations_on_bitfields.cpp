/**
 * @file        test_operations_on_bitfiels.cpp
 * @brief       Tests basic operations on bitfields.
 * @author      Kacper Kowalski - mailprivate
 */
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfields.hpp"

#include "helpers.hpp"

#include <cinttypes>

using namespace jungles;

TEST_CASE("Operations on bitfields for one byte long bitfield", "[operations]")
{
    Bitfields<uint8_t, Field<Reg::field1, 3>, Field<Reg::field2, 2>, Field<Reg::field3, 3>> bf;

    SECTION("Setting most-right field")
    {
        bf.at<Reg::field1>() = 0b010;
        REQUIRE(bf.at<Reg::field1>() == 0b010);
    }

    SECTION("Setting field in the middle")
    {
        bf.at<Reg::field2>() = 0b11;
        REQUIRE(bf.at<Reg::field2>() == 0b11);
    }

    SECTION("Bitwise OR operation")
    {
        bf.at<Reg::field3>() = 0b10;
        bf.at<Reg::field3>() |= 0b11;
        REQUIRE(bf.at<Reg::field3>() == 0b11);
    }

    SECTION("Bitwise bit clearing")
    {
        bf.at<Reg::field3>() = 0b111;
        bf.at<Reg::field3>() &= ~0b010;
        REQUIRE(bf.at<Reg::field3>() == 0b101);
    }
}

TEST_CASE("Operations on bitfields for unsigned as underlying type", "[operations]")
{
    Bitfields<unsigned, Field<Reg::field1, 30>, Field<Reg::field2, 1>, Field<Reg::field3, 1>> bf;

    bf.at<Reg::field1>() = 0b11111111111111111111111111111;
    bf.at<Reg::field1>() &= ~0b00010000000000000000000000000;
    REQUIRE(bf.at<Reg::field1>() == 0b11101111111111111111111111111);
}

TEST_CASE("Operations on bitfields for 8-byte long bitfield", "[operations]")
{
    Bitfields<uint64_t, Field<Reg::field1, 31>, Field<Reg::field2, 5>, Field<Reg::field3, 28>> bf;

    bf.at<Reg::field1>() = 0b1111111111111111111111111111111;
    bf.at<Reg::field1>() &= ~0b0100000000000000000000000000000;
    REQUIRE(bf.at<Reg::field1>() == 0b1011111111111111111111111111111);
}
