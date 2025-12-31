/**
 * @file        test_deserializing.cpp
 * @brief       Tests whether fields are deserialized/loaded properly.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>
#include <limits>

#include "helpers.hpp"
#include "jungles/bitfields.hpp"

using namespace jungles;

TEST_CASE("Fields are deserialized/loaded", "[desieralization]") {
  SECTION("Zero-initialized by default") {
    Bitfields<uint16_t, Field<Reg::field1, 1>, Field<Reg::field2, 1>, Field<Reg::field3, 14>> bf;
    REQUIRE(bf.serialize() == 0);
  }

  SECTION("1-bit size bitfields are loaded") {
    using OneBitSizedBitfieldsRegister =
        Bitfields<uint8_t, Field<Reg::field1, 1>, Field<Reg::field2, 1>, Field<Reg::field3, 1>, Field<Reg::field4, 1>,
                  Field<Reg::field5, 1>, Field<Reg::field6, 1>, Field<Reg::field7, 1>, Field<Reg::field8, 1>>;

    OneBitSizedBitfieldsRegister reg{0b00110101};

    REQUIRE(reg.at<Reg::field1>() == 0);
    REQUIRE(reg.at<Reg::field2>() == 0);
    REQUIRE(reg.at<Reg::field3>() == 1);
    REQUIRE(reg.at<Reg::field4>() == 1);
    REQUIRE(reg.at<Reg::field5>() == 0);
    REQUIRE(reg.at<Reg::field6>() == 1);
    REQUIRE(reg.at<Reg::field7>() == 0);
    REQUIRE(reg.at<Reg::field8>() == 1);
  }

  SECTION("Three bitfields with straddling") {
    Bitfields<uint16_t, Field<Reg::field1, 5>, Field<Reg::field2, 7>, Field<Reg::field3, 4>> bf{0b0010011000110110};
    REQUIRE(bf.at<Reg::field1>() == 0b00100);
    REQUIRE(bf.at<Reg::field2>() == 0b1100011);
    REQUIRE(bf.at<Reg::field3>() == 0b0110);
  }

  SECTION("Getting mask") {
    Bitfields<uint16_t, Field<Reg::field1, 7>, Field<Reg::field2, 5>, Field<Reg::field3, 4>> bf{
        std::numeric_limits<uint16_t>::max()};

    REQUIRE(bf.extract<Reg::field1>() == 0b1111111000000000);
    REQUIRE(bf.extract<Reg::field2>() == 0b0000000111110000);
    REQUIRE(bf.extract<Reg::field3>() == 0b0000000000001111);
  }
}
