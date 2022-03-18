/**
 * @file        bitfield_mapping.hpp
 * @brief       Defines struct which handles bitfield mapping.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BITFIELD_MAPPING_HPP
#define BITFIELD_MAPPING_HPP

namespace jungles
{

template<auto Id, unsigned Size>
struct Field
{
    static inline constexpr auto id{Id};
    static inline constexpr auto size{Size};
};

template<typename... Fields>
struct BitfieldMapping
{
};

}; // namespace jungles

#endif /* BITFIELD_MAPPING_HPP */
