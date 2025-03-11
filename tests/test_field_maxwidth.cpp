
#include "jungles/bitfields.hpp"

using namespace jungles;

int main(void) {
  auto compile_max_width_uint32{[]() {
    enum class SomeEnumClass { V1 };

    Bitfields<uint32_t, Field<SomeEnumClass::V1, 32>>{};
  }};

  auto compile_max_width_uint16{[]() {
    enum class SomeEnumClass { V1 };

    Bitfields<uint16_t, Field<SomeEnumClass::V1, 16>>{};
  }};

  auto compile_max_width_uint8{[]() {
    enum class SomeEnumClass { V1 };

    Bitfields<uint8_t, Field<SomeEnumClass::V1, 8>>{};
  }};

  compile_max_width_uint32();
  compile_max_width_uint16();
  compile_max_width_uint8();

  return 0;
}
