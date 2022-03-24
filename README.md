# Portable Bitfields C++ library

This is a C++20 portable bitfields header-only library with defined behaviour, in the contrary to the standard. 
This library intends to strictly define the behaviour, which the standard leaves for the compilers to 
define. Implements serialization and deserialization of the bitfields, and this is the main objective of this library.

Examples of usage:

* Accessing single bits of registers in ICs, from which the data is obtained through I2C, SPI, UART, ...
* Protocol headers.
* ...

## Features

* Overflow and out-of-range defined bevaviour: least significant bits of the value are masked to fit into the bitfield.
* Bitfields are packed left-to-right, in the same order they are defined.
* Bitfields are packed, thus straddling is supported.
* Configurable padding, by enforcing whole underlying type allocation.
* Implements bitfield serialization, deserialization and extraction of a single bitfield's value, with proper shifting.
* C++20.
* Tested with GCC 11.1 and Clang 13.0.0.
* Supports bitfield groups total length up to 8 bytes.

## Usage

### Basic usage

Define field IDs (names):

```
enum class Id
{
    f1, f2, f3
};
```

Define bitfield type:

```
using namespace jungles;

using UnderlyingType = uint16_t;
using Register = Bitfields<
    UnderlyingType, 
    Field{.id = Id::f1, .size = 3}, // Has also an explicit constructor, so
                                    // this is equivalent: 
                                    // Field{Id::f1, 3}
    Field{.id = Id::f2, .size = 9}, 
    Field{.id = Id::f3, .size = 4}>; 

```

The `UnderlyingType` will be occupied by the single fields like that:

```
f1 = X
f2 = Y
f3 = Z

XXXYYYYYYYYYZZZZ - 16 bits in total, because uint16_t has bit-size equal to 16
```

The above code shall emulate a structure like this:

```
struct RegisterWithRawBitfields
{
    uint16_t f1 : 3;
    uint16_t f2 : 9;
    uint16_t f3 : 4;
};
```

Use the bitfields:

```
Register r; // Default zero-initialized.
r.at<Id::f1>() = 0b101;
r.at<Id::f2>() = 0b001111100;
r.at<Id::f3>() = 0b0110;

// Emulates that:
// RegisterWithRawBitfields r_raw;
// r_raw.f1 = 0b101; 
// r_raw.f2 = 0b001111100; 
// r_raw.f3 = 0b0110; 

ASSERT(r.extract<Id::f1>() == 0b1010000000000000);
ASSERT(r.extract<Id::f2>() == 0b0000011111000000);
ASSERT(r.extract<Id::f3>() == 0b0000000000000110);

ASSERT(r.serialize() == 0b1010011111000110);

```

Deserialize:

```
Register r{0b0101110001110110};
//           XXXYYYYYYYYYZZZZ
ASSERT(r.at<Id::f1>() == 0b010);
ASSERT(r.at<Id::f2>() == 0b111000111);
ASSERT(r.at<Id::f3>() == 0b0110);
```

### Example use case #1: RTP header's first word

```
enum class RtpHeaderField
{
    version, padding, extension, csrc_count, marker, payload_type, sequence_number
};

using namespace jungles;
using RtpHeaderFirstWord = Bitfields<
    uint32_t,
    Field{RtpHeaderField::version,              2},
    Field{RtpHeaderField::padding,              1},
    Field{RtpHeaderField::extension,            1},
    Field{RtpHeaderField::csrc_count,           4},
    Field{RtpHeaderField::marker,               1},
    Field{RtpHeaderField::payload_type,         7},
    Field{RtpHeaderField::sequence_number,      16}>;

RtpHeaderFirstWord rtp_header_first_word;
```

### Example use case #2: Zigbee ZCL header field Frame Format

```
enum class FrameFormatFields
{
    frame_type, manufacturer_specific, direction, disable_default_response, reserved
};

using namespace jungles;
using ZclFrameFormat = Bitfields,
    uint32_t,
    Field{FrameFormatFields::frame_type,               2},
    Field{FrameFormatFields::manufacturer_specific,    1},
    Field{FrameFormatFields::direction,                1},
    Field{FrameFormatFields::disable_default_response, 1},
    Field{FrameFormatFields::reserved,                 2}>;
```

### Example use case #3: IC MP2695 Status register

```
enum class StatusField
{
    reserved1 = 0,
    chg_stat,
    vppm_stat,
    ippm_stat,
    usb1_plug_in,
    reserved2
};

using namespace jungles;
using Status = Bitfields<uint8_t, 
                         Field{StatusField::reserved1,    2}, 
                         Field{StatusField::chg_stat,     2},
                         Field{StatusField::vppm_stat,    1},
                         Field{StatusField::ippm_stat,    1},
                         Field{StatusField::usb1_plug_in, 1},
                         Field{StatusField::reserved2,    1}>;
```

### Interface

#### at()

#### serialize()

#### extract()

### Constraints, expected behaviour, tips and other notes

#### 1. Overflow, or out-of-range

#### 2. Field ID type

* Field ID types must be the same for each field.
* Supported types are integral types, enum classes, enums, or other "structural" type.

#### 3. Field ID

* Field IDs must not duplicate.
* Accessing non-existing field ID is an error.

#### 4. Field sizes

#### 5. Padding

Use "reserved" fields.

#### 6. Mask

# Testing

`JUNGLES_BITFIELD_ENABLE_TESTING`
`JUNGLES_BITFIELD_ENABLE_PORTABILITY_TESTS` - makes it slow

# To research:

1. Configurable field ordering, e.g. allow right-to-left field ordering.
2. If Ad.1 legit, then switching ordering at runtime useful?
3. Configurable overflow policies, e.g. allow, throw, clear field, etc.
4. Would it be useful, if the type of a single field could be different than the underlying type of the bitfield group?
5. Would it be useful, if the types of the field IDs wouldn't have to be the same?

# Todos:

1. Implement bitfield loading from already serialized value.
2. Implement to `std::array` serialization.
3. Allow underlying type `std::array`.
configuration. This doesn't work due to compilation error from Catch2.
4. Turn above todos into issues.
5. Create ToC.
