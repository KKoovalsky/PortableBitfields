/**
 * @file        test_const.cpp
 * @brief       Tests handling of const Bitfields
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "helpers.hpp"
#include "jungles/bitfields.hpp"

using namespace jungles;

using Bf = Bitfields<uint16_t, Field<Reg::field1, 2>, Field<Reg::field2, 8>,
                     Field<Reg::field3, 6>>;

TEST_CASE("Const bitfields", "[const]") {
  SECTION("Reading bitfields") {
    const Bf bf{0b1001101001101010};
    REQUIRE(bf.at<Reg::field1>() == 0b10);
    REQUIRE(bf.at<Reg::field2>() == 0b01101001);
    REQUIRE(bf.at<Reg::field3>() == 0b101010);
  }

  SECTION("Extracting fields") {
    const Bf bf{0b0110010110010101};
    REQUIRE(bf.extract<Reg::field1>() == 0b0100000000000000);
    REQUIRE(bf.extract<Reg::field2>() == 0b0010010110000000);
    REQUIRE(bf.extract<Reg::field3>() == 0b0000000000010101);
  }

  SECTION("Serializing bitfields") {
    {
      const Bf bf{0b1001101001101010};
      REQUIRE(bf.serialize() == 0b1001101001101010);
    }
    {
      const Bf bf{0b0110010110010101};
      REQUIRE(bf.serialize() == 0b0110010110010101);
    }
  }

  SECTION("Referencing bitfields") {
    SECTION("Referencing a const bitfield") {
      const Bf bf{0b1001101001101010};
      const auto& f2{bf.at<Reg::field2>()};
      REQUIRE(f2 == 0b01101001);
    }

    SECTION("Const-reference to a field") {
      Bf bf{0b1001101001101010};
      const auto& f2{bf.at<Reg::field2>()};
      REQUIRE(f2 == 0b01101001);
    }
  }
}
