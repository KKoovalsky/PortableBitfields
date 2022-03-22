/**
 * @file        test_zero_initialized.cpp
 * @brief       Tests whether the Bitfield is zero-initialized
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfield.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Bitfields are zero-initialized", "[misc]")
{
    Bitfield<uint16_t, ByteOrder::little, Field{Reg::field1, 1}, Field{Reg::field2, 1}, Field{Reg::field3, 14}> bf;
    REQUIRE(bf.serialize() == 0);
}
