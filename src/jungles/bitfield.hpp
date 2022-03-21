/**
 * @file        bitfield.hpp
 * @brief       Bitfield library implementation.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BITFIELD_HPP
#define BITFIELD_HPP

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace jungles
{

enum class ByteOrder
{
    big,
    little
};

template<typename IdType>
struct Field
{
    const IdType id;
    const unsigned size;
};

template<std::integral UnderlyingType, ByteOrder ByteOrder_, auto... Fields>
class Bitfield
{
  public:
    template<auto FieldId>
    constexpr UnderlyingType& at()
    {
        constexpr auto idx{find_field_index<FieldId>()};
        return field_values[idx];
    }

    template<auto FieldId>
    constexpr UnderlyingType extract() const
    {
        constexpr auto idx{find_field_index<FieldId>()};
        constexpr auto shift{field_shifts[idx]};
        auto v{field_values[idx]};
        auto result{static_cast<UnderlyingType>(v << shift)};
        if constexpr (ByteOrder_ == ByteOrder::big or UnderlyingTypeSize == 1)
            return result;
        else
            return to_little_endian(result);
    }

    constexpr UnderlyingType serialize() const
    {
        return (extract<Fields.id>() | ... | 0);
    }

  private:
    static inline constexpr auto to_field_ids()
    {
        using IdType = std::decay_t<decltype(std::decay_t<decltype(fields)>::value_type::id)>;
        std::array<IdType, NumberOfFields> ids;
        std::transform(std::begin(fields), std::end(fields), std::begin(ids), [](auto f) { return f.id; });
        return ids;
    }

    static inline constexpr auto to_field_sizes()
    {
        std::array<unsigned, NumberOfFields> sizes;
        std::transform(std::begin(fields), std::end(fields), std::begin(sizes), [](auto f) { return f.size; });
        return sizes;
    }

    static inline constexpr auto to_field_shifts()
    {
        std::array<unsigned, NumberOfFields> shifts;

        auto shifts_it{std::rbegin(shifts)};
        auto sizes_it{std::rbegin(field_sizes)};

        unsigned accumulated_field_size{0};
        while (shifts_it != std::rend(shifts))
        {
            *shifts_it++ = accumulated_field_size;
            accumulated_field_size += *sizes_it++;
        }

        return shifts;
    }

    template<auto FieldId>
    static inline constexpr auto find_field_index()
    {
        constexpr auto it{std::find(std::begin(field_ids), std::end(field_ids), FieldId)};
        static_assert(it != std::end(field_ids), "Field ID not found");
        return static_cast<unsigned>(std::distance(std::begin(field_ids), it));
    }

    static inline constexpr auto to_little_endian(UnderlyingType big_endian_value)
    {
        constexpr auto underlying_type_byte_size{sizeof(UnderlyingType)};
        constexpr auto underlying_type_bit_size{underlying_type_byte_size * 8};

        UnderlyingType result{0};

        auto in_shift{underlying_type_bit_size - 8};
        auto out_shift{0};

        for (unsigned i{0}; i < underlying_type_byte_size; ++i)
        {
            result |= ((big_endian_value >> in_shift) & 0xFF) << out_shift;
            in_shift -= 8;
            out_shift += 8;
        }

        return result;
    }

    static inline constexpr unsigned NumberOfFields{sizeof...(Fields)};
    static inline constexpr unsigned UnderlyingTypeSize{sizeof(UnderlyingType)};

    static inline constexpr std::array fields{Fields...};
    static inline constexpr auto field_ids{to_field_ids()};
    static inline constexpr auto field_sizes{to_field_sizes()};
    static inline constexpr auto field_shifts{to_field_shifts()};

    std::array<UnderlyingType, NumberOfFields> field_values = {};
};

} // namespace jungles

#endif /* BITFIELD_HPP */
