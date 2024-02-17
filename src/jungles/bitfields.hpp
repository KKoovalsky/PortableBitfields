/**
 * @file        bitfields.hpp
 * @brief       Bitfield library implementation.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BITFIELDS_HPP
#define BITFIELDS_HPP

#include <array>
#include <climits>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace jungles
{

namespace detail
{
template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value)
{
    for (; first != last; ++first)
    {
        if (*first == value)
        {
            return first;
        }
    }
    return last;
}

template<class InputIt, class T>
constexpr T accumulate(InputIt first, InputIt last, T init)
{
    for (; first != last; ++first)
    {
        init = std::move(init) + *first;
    }
    return init;
}

template<class InputIt, class OutputIt, class UnaryOperation>
constexpr OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op)
{
    while (first1 != last1)
    {
        *d_first++ = unary_op(*first1++);
    }
    return d_first;
}

} // namespace detail

template<auto Id, unsigned Size>
struct Field
{
    static inline constexpr auto id{Id};
    static inline constexpr auto size{Size};
};

template<typename UnderlyingType, typename... Fields>
class Bitfields
{
  public:
    constexpr Bitfields() = default;

    constexpr Bitfields(UnderlyingType preload)
    {
        for (unsigned i{0}; i < NumberOfFields; ++i)
        {
            auto mask{field_masks[i]};
            auto masked_value{mask & preload};
            auto shift{field_shifts[i]};
            field_values[i] = masked_value >> shift;
        }
    }

    template<auto FieldId>
    constexpr UnderlyingType& at() noexcept
    {
        constexpr auto idx{find_field_index<FieldId>()};
        UnderlyingType& result{field_values[idx]};
        result &= non_shifted_field_masks[idx];
        return result;
    }

    template<auto FieldId>
    constexpr const UnderlyingType& at() const noexcept
    {
        constexpr auto idx{find_field_index<FieldId>()};
        UnderlyingType& result{field_values[idx]};
        result &= non_shifted_field_masks[idx];
        return result;
    }

    template<auto FieldId>
    constexpr UnderlyingType extract() const noexcept
    {
        constexpr auto idx{find_field_index<FieldId>()};
        constexpr auto shift{field_shifts[idx]};
        constexpr auto mask{non_shifted_field_masks[idx]};
        auto v{field_values[idx] & mask};
        auto result{static_cast<UnderlyingType>(v << shift)};
        return result;
    }

    constexpr UnderlyingType serialize() const noexcept
    {
        return (extract<Fields::id>() | ... | 0);
    }

  private:
    static inline constexpr auto to_field_shifts() noexcept
    {
        std::array<unsigned, NumberOfFields> shifts = {};

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

    static inline constexpr auto to_non_shifted_field_masks() noexcept
    {
        std::array<UnderlyingType, NumberOfFields> field_masks = {};

        for (unsigned i{0}; i < NumberOfFields; ++i)
        {
            auto field_size{static_cast<UnderlyingType>(field_sizes[i])};
            // Funny! This is needed, because if we write: (1 << field_size) compiler will use type 'int' for one,
            // and shifting (int << uint64_t) is undefined behavior; see:
            // https://stackoverflow.com/questions/10499104/is-shifting-more-than-32-bits-of-a-uint64-t-integer-on-an-x86-machine-undefined#answer-10499371
            auto one{static_cast<UnderlyingType>(1)};
            field_masks[i] = (one << field_size) - 1;
        }

        return field_masks;
    }

    static inline constexpr auto to_shifted_field_masks() noexcept
    {
        std::array<UnderlyingType, NumberOfFields> masks = {};

        for (unsigned i{0}; i < NumberOfFields; ++i)
        {
            auto mask{non_shifted_field_masks[i]};
            auto shift{field_shifts[i]};
            masks[i] = mask << shift;
        }

        return masks;
    }

    template<auto FieldId>
    static inline constexpr auto find_field_index() noexcept
    {
        constexpr auto it{detail::find(std::begin(field_ids), std::end(field_ids), FieldId)};
        static_assert(it != std::end(field_ids), "Field ID not found");
        return static_cast<unsigned>(std::distance(std::begin(field_ids), it));
    }

    static inline constexpr bool has_duplicates()
    {
        auto beg{std::begin(field_ids)}, end{std::end(field_ids)};

        for (auto it{beg}; it != end; ++it)
        {
            auto match_it{detail::find(std::next(it), end, *it)};
            if (match_it != end)
                return true;
        }

        return false;
    }

    static inline constexpr unsigned calculate_occupied_bit_size()
    {
        return detail::accumulate(std::begin(field_sizes), std::end(field_sizes), 0u);
    }

    static inline constexpr unsigned NumberOfFields{sizeof...(Fields)};
    static inline constexpr unsigned UnderlyingTypeSize{sizeof(UnderlyingType)};
    static inline constexpr unsigned UnderlyingTypeBitSize{UnderlyingTypeSize * CHAR_BIT};

    static inline constexpr std::array field_ids{Fields::id...};
    static inline constexpr std::array field_sizes{Fields::size...};
    static inline constexpr auto field_shifts{to_field_shifts()};
    static inline constexpr auto non_shifted_field_masks{to_non_shifted_field_masks()};
    static inline constexpr auto field_masks{to_shifted_field_masks()};

    static_assert(std::is_integral<UnderlyingType>::value, "UnderlyingType must be an integral type");
    static_assert(!has_duplicates(), "Field IDs must not duplicate");
    static_assert(calculate_occupied_bit_size() == UnderlyingTypeBitSize,
                  "Accumulated bit size is not equal to underlying type's bit size");

    std::array<UnderlyingType, NumberOfFields> field_values = {};
};

} // namespace jungles

#endif /* BITFIELDS_HPP */
