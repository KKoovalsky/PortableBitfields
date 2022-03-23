# Portable Bitfields C++ library

This is a C++20 portable bitfields library, endian-configurable and with defined behaviour, in the contrary to the
standard. This library intends to strictly define the behaviour, which the standard leaves for the compilers to define.
Implements serialization of the bitfields, and this is the main objective of this library.

## Features

* Overflow and out-of-range defined bevaviour: least significant bits of the value are masked to fit into the bitfield.
* Bitfields are packed left-to-right, in the same order they are defined.
* Choice of endianness.
* Bitfields are packed, thus straddling is supported.
* Configurable padding, by enforcing whole underlying type allocation.
* Implements bitfield serialization amd extraction of a single bitfield's value, with proper shifting.
* C++20.
* Tested with GCC 11.1 and Clang 13.0.0.

## Usage

### Basic usage

Define field IDs (names):

```
enum class Id
{
    f1, f2, f3
};
```

Define bitfield type;

```
using namespace jungles;

using UnderlyingType = uint16_t;
using Register = Bitfields<
    UnderlyingType, 
    ByteOrder::big,
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
Register r;
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

### Use case #1: some simple TCP/IP protocol header

### Use case #2: some CAN, MODBUS, BLE, Thread, ZigBee header

### Use case #3: MP2695 Status register

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
2. Switching endianness at runtime useful?
3. If Ad.1 legit, then switching ordering at runtime useful?
4. Configurable overflow policies, e.g. allow, throw, clear field, etc.
5. Would it be useful, if the type of a single field could be different than the underlying type of the bitfield group?
6. Would it be useful, if the types of the field IDs wouldn't have to be the same?

# Todos:

1. Implement to `std::array` serialization.
2. Allow underlying type `std::array`.
3. Any test with one-byte-long bitfield group, shall be templated test case, with big endian and  little endian 
configuration. This doesn't work due to compilation error from Catch2.
4. Turn above todos into issues.
5. Create ToC.
