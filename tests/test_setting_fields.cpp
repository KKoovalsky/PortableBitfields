/**
 * @file        test_setting_fields.cpp
 * @brief       Tests setting of the fields in a bitfield.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfield.hpp"

enum class Reg
{
    field1,
    field2,
    field3
};

TEST_CASE("Bit fields are set", "[set]")
{
    using namespace jungles;
    using Cfg = BitfieldConfig<ByteOrder::little, FieldOrder::right_to_left, UnderlyingType<uint8_t>>;
    [[maybe_unused]] constexpr Cfg cfg;
    using Mapping = BitfieldMapping<Field<Reg::field1, 3>, Field<Reg::field2, 3>, Field<Reg::field3, 2>>;
    // Bitfield<Cfg, Mapping> bf;
    //
    // bf.field<Ref::field1> |= 0b011;
    // bf |= Field<Reg::field1>{0b010};
}
