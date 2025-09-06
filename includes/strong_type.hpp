// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include "concepts/arithmetic_operators.hpp"
#include "concepts/assignment_operators.hpp"
#include "concepts/comparison_operators.hpp"
#include "concepts/increment_and_decrement_operators.hpp"
#include "concepts/iterators.hpp"
#include "concepts/logical_operators.hpp"
#include "concepts/other_operators.hpp"
#include "concepts/stl_utils.hpp"
#include "traits/traits.hpp"
#include <utility>

namespace stronger
{

enum class options
{
    allow_implicit_construction,
    allow_implicit_conversion_to_underlying_type,
    drill_down,
};

/**
 *
 * @tparam T Underlying type you want to make strong
 * @tparam Tag Makes different strong types on the same underlying type different.
 *             You can use tag() to set this parameter
 * @tparam Options options to enable
 */
template <typename T, size_t Tag, options... Options>
class strong_type
{
    using const_reference_like = std::conditional_t<traits::should_pass_by_value<T>, strong_type, const strong_type&>;
    using reference = strong_type&;
    template <options Option>
    constexpr static bool option_enabled = ((Option == Options) || ...);
    template <options Option>
    constexpr static bool option_disabled = !option_enabled<Option>;
    using enum options;

public:

    using underlying_type = T;

    strong_type()
        requires concepts::supports_default_construction<T>
    = default;

    constexpr explicit(option_disabled<allow_implicit_construction>) strong_type(T&& value) :
            m_value{ std::forward<T>(value) }
    {
    }

    template <typename... Args>
    constexpr explicit strong_type(std::in_place_t, Args&&... args) : m_value{ std::forward<Args>(args)... }
    {
    }

    constexpr decltype(auto) operator*(this auto&& self) { return std::forward_like<decltype(self)>(self.m_value); }

    constexpr decltype(auto) operator->(this auto&& self)
    {
        if constexpr(std::is_pointer_v<underlying_type> || option_enabled<drill_down>)
            return std::forward_like<decltype(self)>(self.m_value);
        else
            return std::addressof(self.m_value);
    }

    // Implicit conversions ............................................................................................

    [[nodiscard]] explicit(false) constexpr operator const underlying_type&() const&
        requires option_enabled<allow_implicit_conversion_to_underlying_type>
    {
        return m_value;
    }

    [[nodiscard]] explicit(false) constexpr operator underlying_type&() &
        requires option_enabled<allow_implicit_conversion_to_underlying_type>
    {
        return m_value;
    }

    [[nodiscard]] explicit(false) constexpr operator underlying_type&&() &&
        requires option_enabled<allow_implicit_conversion_to_underlying_type>
    {
        return std::move(m_value);
    }

    // Arithmetic operators ............................................................................................

    [[nodiscard]] constexpr friend strong_type operator+(const_reference_like rhs)
        requires concepts::supports_unary_plus<T>
    {
        return strong_type{ +rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator-(const_reference_like rhs)
        requires concepts::supports_unary_minus<T>
    {
        return strong_type{ -rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator+(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_addition<T>
    {
        return strong_type{ lhs.m_value + rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator-(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_subtraction<T>
    {
        return strong_type{ lhs.m_value - rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator*(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_multiplication<T>
    {
        return strong_type{ lhs.m_value * rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator/(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_division<T>
    {
        return strong_type{ lhs.m_value / rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator%(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_remainder<T>
    {
        return strong_type{ lhs.m_value % rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator~(const_reference_like rhs)
        requires concepts::supports_bitwise_not<T>
    {
        return strong_type{ ~rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator&(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_and<T>
    {
        return strong_type{ lhs.m_value & rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator|(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_or<T>
    {
        return strong_type{ lhs.m_value | rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator^(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_xor<T>
    {
        return strong_type{ lhs.m_value ^ rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator<<(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_left_shift<T>
    {
        return strong_type{ lhs.m_value << rhs.m_value };
    }

    [[nodiscard]] constexpr friend strong_type operator>>(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_right_shift<T>
    {
        return strong_type{ lhs.m_value >> rhs.m_value };
    }

    // Assignment operators ............................................................................................

    constexpr friend reference operator+=(reference lhs, const_reference_like rhs)
        requires concepts::supports_addition_assignment<T>
    {
        lhs.m_value += rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator-=(reference lhs, const_reference_like rhs)
        requires concepts::supports_subtraction_assignment<T>
    {
        lhs.m_value -= rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator*=(reference lhs, const_reference_like rhs)
        requires concepts::supports_multiplication_assignment<T>
    {
        lhs.m_value *= rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator/=(reference lhs, const_reference_like rhs)
        requires concepts::supports_division_assignment<T>
    {
        lhs.m_value /= rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator%=(reference lhs, const_reference_like rhs)
        requires concepts::supports_remainder_assignment<T>
    {
        lhs.m_value %= rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator&=(reference lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_and_assignment<T>
    {
        lhs.m_value &= rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator|=(reference lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_or_assignment<T>
    {
        lhs.m_value |= rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator^=(reference lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_xor_assignment<T>
    {
        lhs.m_value ^= rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator<<=(reference lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_left_shift_assignment<T>
    {
        lhs.m_value <<= rhs.m_value;
        return lhs;
    }

    constexpr friend reference operator>>=(reference lhs, const_reference_like rhs)
        requires concepts::supports_bitwise_right_shift_assignment<T>
    {
        lhs.m_value >>= rhs.m_value;
        return lhs;
    }

    // Increment and decrement operators ...............................................................................

    constexpr reference operator++()
        requires concepts::supports_pre_increment<T>
    {
        ++m_value;
        return *this;
    }

    constexpr reference operator--()
        requires concepts::supports_pre_decrement<T>
    {
        --m_value;
        return *this;
    }

    constexpr strong_type operator++(int)
        requires concepts::supports_post_increment<T>
    {
        return strong_type{ m_value++ };
    }

    constexpr strong_type operator--(int)
        requires concepts::supports_post_decrement<T>
    {
        return strong_type{ m_value-- };
    }

    // Comparison operators ............................................................................................

    [[nodiscard]] constexpr friend bool operator==(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_equal_to<T>
    {
        return lhs.m_value == rhs.m_value;
    }

    [[nodiscard]] constexpr friend bool operator!=(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_not_equal_to<T>
    {
        return lhs.m_value != rhs.m_value;
    }

    [[nodiscard]] constexpr friend bool operator<(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_less_than<T>
    {
        return lhs.m_value < rhs.m_value;
    }

    [[nodiscard]] constexpr friend bool operator>(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_greater_than<T>
    {
        return lhs.m_value > rhs.m_value;
    }

    [[nodiscard]] constexpr friend bool operator<=(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_less_than_or_equal_to<T>
    {
        return lhs.m_value <= rhs.m_value;
    }

    [[nodiscard]] constexpr friend bool operator>=(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_greater_than_or_equal_to<T>
    {
        return lhs.m_value >= rhs.m_value;
    }

    [[nodiscard]] constexpr friend auto operator<=>(const_reference_like lhs, const_reference_like rhs)
        requires concepts::supports_three_way_comparison<T>
    {
        return lhs.m_value <=> rhs.m_value;
    }

    // Logical operators ...............................................................................................

    [[nodiscard]] constexpr friend bool operator!(const_reference_like rhs)
        requires concepts::supports_negation<T>
    {
        return !rhs.m_value;
    }

    // operator&& and operator|| are not implemented because short-circuiting evaluation wouldn't be supported.
    // To perform these operations, use *a && *b instead of a&&b, or *a || *b instead of a&&b

    // Other operators .................................................................................................

    template <typename... Args>
    [[nodiscard]] static constexpr decltype(auto) operator()(Args&&... args)
        requires concepts::supports_static_call<underlying_type, Args...>
    {
        return underlying_type::operator()(std::forward<Args>(args)...);
    }

    template <typename... Args>
    [[nodiscard]] constexpr decltype(auto) operator()(this auto&& self, Args&&... args)
        requires concepts::supports_non_static_call<underlying_type, Args...>
    {
        return self.m_value(std::forward<Args>(args)...);
    }

    template <typename... Args>
    [[nodiscard]] static constexpr decltype(auto) operator[](Args&&... args)
        requires concepts::supports_static_subscript<underlying_type, Args...>
    {
        return underlying_type::operator[](std::forward<Args>(args)...);
    }

    template <typename... Args>
    [[nodiscard]] constexpr decltype(auto) operator[](this auto&& self, Args&&... args)
        requires concepts::supports_non_static_subscript<underlying_type, Args...>
    {
        return self.m_value[std::forward<Args>(args)...];
    }

    // Ostream insertion ...............................................................................................

    constexpr friend std::ostream& operator<<(std::ostream& os, const_reference_like rhs)
        requires concepts::supports_ostream_insertion<T>
    {
        return os << rhs.m_value;
    }

    [[nodiscard]] constexpr decltype(auto) begin(this auto&& self)
        requires concepts::supports_begin<T>
    {
        return std::forward<decltype(self)>(self).m_value.begin();
    }

    [[nodiscard]] constexpr decltype(auto) end(this auto&& self)
        requires concepts::supports_end<T>
    {
        return std::forward<decltype(self)>(self).m_value.end();
    }

private:

    T m_value;
#ifdef STRONGER_CPP_BUILD_TESTS
public:

    struct info
    {
        using get_underlying_type = underlying_type;
        using get_const_reference_like = const_reference_like;
        using get_reference = reference;
    };
#endif
};

/// Utility alias for strong bools that allows implicit conversion to bool
template <size_t Tag>
using strong_bool = strong_type<bool, Tag, options::allow_implicit_conversion_to_underlying_type>;

/**
 * Constructs a strong type of type `StrongType` in-place from arguments of the underlying type's constructor.
 *
 * @tparam StrongType Target strong_type class
 * @tparam Args Type of arguments to forward to StrongType::underlying_type constructor
 * @param args Arguments to forward to StrongType::underlyint_type constructor
 */
template <typename StrongType, typename... Args>
[[nodiscard]] constexpr StrongType make_strong(Args&&... args)
{
    return StrongType{ std::in_place, std::forward<Args>(args)... };
}

/**
 * Construct a templated StrongType deducing the template argument from make_strong arguments
 * Typically, this is useful when working on strong types of Callables, that need to be initialized with lambdas
 */
template <template <typename T> class StrongType, typename T>
[[nodiscard]] constexpr StrongType<T> make_strong(T&& value)
{
    return StrongType<T>{ std::forward<T>(value) };
}

}  // namespace stronger

template <stronger::concepts::supports_std_format T, size_t Tag, stronger::options... Options>
struct std::formatter<stronger::strong_type<T, Tag, Options...>> : std::formatter<T>
{
    auto format(const stronger::strong_type<T, Tag, Options...>& s, std::format_context& ctx) const
    {
        return std::formatter<T>::format(*s, ctx);
    }
};

template <stronger::concepts::supports_std_hash T, size_t Tag, stronger::options... Options>
struct std::hash<stronger::strong_type<T, Tag, Options...>>
{
    constexpr hash() = default;
    static constexpr auto operator()(const stronger::strong_type<T, Tag, Options...>& s) noexcept
    {
        return std::hash<T>{}(*s);
    }
};
