/**
 * @file        test_little_endian_serializing.cpp
 * @brief       Tests whether bitfields are properly serialized to Little Endian.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfield.hpp"

BITFIELD(OneByteBitfield, (one, 2), (two, 3), (three, 3));
RTTR_REGISTRATION
{
    rttr::registration::class_<OneByteBitfield>("OneByteBitfield")
        .constructor<>()
        .property("value", &OneByteBitfield::value);
}

TEST_CASE("Single byte bitfield is serialized to little endian", "[little_endian]")
{
    OneByteBitfield bf{.one = 0b10, .two = 0b101, .three = 0b011};
    // REQUIRE(bf.to_little_endian() == 0b10101011);
}
