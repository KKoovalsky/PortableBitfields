/**
 * @file        bitfield_config.hpp
 * @brief       Bitfield configuration helper types.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BITFIELD_CONFIG_HPP
#define BITFIELD_CONFIG_HPP

#include <type_traits>

namespace jungles
{

enum class ByteOrder
{
    big,
    little
};

enum class FieldOrder
{
    right_to_left,
    left_to_right
};

template<typename T>
struct UnderlyingType
{
    using type = T;
};

template<typename>
struct IsUnderlyingTypeInstantiation : std::false_type
{
};

template<typename T>
struct IsUnderlyingTypeInstantiation<UnderlyingType<T>> : std::true_type
{
};

template<typename T>
concept UnderlyingTypeInstantation = IsUnderlyingTypeInstantiation<T>::value;

template<ByteOrder ByteOrder_, FieldOrder FieldOrder_, UnderlyingTypeInstantation UnderlyingType_>
struct BitfieldConfig
{
};

#endif /* BITFIELD_CONFIG_HPP */
