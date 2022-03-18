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
enum class FieldOrder
{
    right_to_left,
    left_to_right
};

template<typename IdType>
struct Field
{
    const IdType id;
    const unsigned size;
};

template<std::integral UnderlyingType, ByteOrder ByteOrder_, FieldOrder FieldOrder_, auto... Fields>
class Bitfield
{
  private:
    static inline constexpr unsigned NumberOfFields{sizeof...(Fields)};

  public:
    template<auto FieldId>
    UnderlyingType& at()
    {
        auto idx{find_field_index<FieldId>()};
        return field_values[idx];
    }

  private:
    static constexpr auto to_field_ids(const auto& fields_)
    {
        using IdType = std::decay_t<decltype(std::decay_t<decltype(fields_)>::value_type::id)>;
        std::array<IdType, NumberOfFields> ids;
        std::transform(std::begin(fields), std::end(fields), std::begin(ids), [](auto f) { return f.id; });
        return ids;
    }

    template<auto FieldId>
    constexpr auto find_field_index() const
    {
        constexpr auto it{std::find(std::begin(field_ids), std::end(field_ids), FieldId)};
        static_assert(it != std::end(field_ids), "Field ID not found");
        return static_cast<unsigned>(std::distance(std::begin(field_ids), it));
    }

    static inline constexpr std::array fields{Fields...};
    static inline constexpr auto field_ids{to_field_ids(fields)};

    std::array<UnderlyingType, NumberOfFields> field_values;
};

} // namespace jungles

#endif /* BITFIELD_HPP */
