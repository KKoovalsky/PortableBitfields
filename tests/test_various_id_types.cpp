/**
 * @file	test_various_id_types.cpp
 * @brief	Creates bitfields with various ID types, to check whether it compiles.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "jungles/bitfields.hpp"

using namespace jungles;

int main(void)
{
    auto compile_enum_as_id{[]() {
        enum SomeEnum
        {
            EnumVal1,
            EnumVal2,
            EnumVal3
        };

        Bitfields<uint8_t, Field<EnumVal1, 2>, Field<EnumVal2, 3>, Field<EnumVal3, 3>>{};
    }};

    auto compile_int_as_id{[]() {
        Bitfields<uint8_t, Field<10, 2>, Field<20, 3>, Field<30, 3>>{};
    }};

    auto compile_enum_class_as_id{[]() {
        enum class SomeEnumClass
        {
            V1,
            V2,
            V3
        };

        Bitfields<uint8_t, Field<SomeEnumClass::V1, 2>, Field<SomeEnumClass::V2, 3>, Field<SomeEnumClass::V3, 3>>{};
    }};

    compile_enum_as_id();
    compile_int_as_id();
    compile_enum_class_as_id();

    return 0;
}
