// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include <iosfwd>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <string>
#include <format>

namespace stronger::tests
{

enum class Operation
{
    noop,

    // Arithmetic operations
    unary_plus,
    unary_minus,
    addition,
    subtraction,
    multiplication,
    division,
    remainder,
    bitwise_not,
    bitwise_and,
    bitwise_or,
    bitwise_xor,
    bitwise_left_shift,
    bitwise_right_shift,

    // Assignment operations
    addition_assignment,
    subtraction_assignment,
    multiplication_assignment,
    division_assignment,
    remainder_assignment,
    bitwise_and_assignment,
    bitwise_or_assignment,
    bitwise_xor_assignment,
    bitwise_left_shift_assignment,
    bitwise_right_shift_assignment,

    // Comparison operations
    equal_to,
    not_equal_to,
    less_than,
    greater_than,
    less_than_or_equal_to,
    greater_than_or_equal_to,
    three_way_comparison,

    // Increment and decrement operations
    pre_increment,
    pre_decrement,
    post_increment,
    post_decrement,

    // Logical operations
    negation,

    // Other operators
    static_call,
    call,
    static_subscript,
    subscript,

    // Iterators
    begin_function,
    end_function,

    // Stream
    ostream_insertion,

    // Others
    hash,
    format
};

struct instruction
{
    static constexpr int ostream_insertion_id = 123456789;
    constexpr bool operator==(const instruction& other) const = default;
    constexpr instruction(const Operation op_, const std::optional<int> lhs_, const std::optional<int> rhs_) :
            op(op_),
            lhs(lhs_),
            rhs(rhs_)
    {
    }

    constexpr instruction(const Operation op_, const std::ostream&, const std::optional<int> rhs_) :
            op(op_),
            lhs(ostream_insertion_id),
            rhs(rhs_)
    {
    }

    Operation op;
    std::optional<int> lhs;
    std::optional<int> rhs;
};

struct mock_type
{
    constexpr mock_type(const int value_, std::vector<instruction>* instructions_) :
            value(value_),
            instructions(instructions_)
    {
    }

    constexpr explicit(false) mock_type(const Operation origin_) : origin(origin_) {}

    constexpr mock_type() = default;

    using enum Operation;
    [[nodiscard]] constexpr friend mock_type operator+(const mock_type& rhs)
    {
        rhs.instructions->emplace_back(unary_plus, std::nullopt, rhs.value);
        return { unary_plus };
    }

    [[nodiscard]] constexpr friend mock_type operator-(const mock_type& rhs)
    {
        rhs.instructions->emplace_back(unary_minus, std::nullopt, rhs.value);
        return { unary_minus };
    }

    [[nodiscard]] constexpr friend mock_type operator+(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(addition, lhs.value, rhs.value);
        return { addition };
    }

    [[nodiscard]] constexpr friend mock_type operator-(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(subtraction, lhs.value, rhs.value);
        return { subtraction };
    }

    [[nodiscard]] constexpr friend mock_type operator*(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(multiplication, lhs.value, rhs.value);
        return { multiplication };
    }

    [[nodiscard]] constexpr friend mock_type operator/(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(division, lhs.value, rhs.value);
        return { division };
    }

    [[nodiscard]] constexpr friend mock_type operator%(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(remainder, lhs.value, rhs.value);
        return { remainder };
    }

    [[nodiscard]] constexpr friend mock_type operator~(const mock_type& rhs)
    {
        rhs.instructions->emplace_back(bitwise_not, std::nullopt, rhs.value);
        return { bitwise_not };
    }

    [[nodiscard]] constexpr friend mock_type operator&(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_and, lhs.value, rhs.value);
        return { bitwise_and };
    }

    [[nodiscard]] constexpr friend mock_type operator|(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_or, lhs.value, rhs.value);
        return { bitwise_or };
    }

    [[nodiscard]] constexpr friend mock_type operator^(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_xor, lhs.value, rhs.value);
        return { bitwise_xor };
    }

    [[nodiscard]] constexpr friend mock_type operator<<(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_left_shift, lhs.value, rhs.value);
        return { bitwise_left_shift };
    }

    [[nodiscard]] constexpr friend mock_type operator>>(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_right_shift, lhs.value, rhs.value);
        return { bitwise_right_shift };
    }

    // Assignment operators ............................................................................................

    constexpr friend mock_type& operator+=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(addition_assignment, lhs.value, rhs.value);
        lhs.origin = addition_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator-=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(subtraction_assignment, lhs.value, rhs.value);
        lhs.origin = subtraction_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator*=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(multiplication_assignment, lhs.value, rhs.value);
        lhs.origin = multiplication_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator/=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(division_assignment, lhs.value, rhs.value);
        lhs.origin = division_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator%=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(remainder_assignment, lhs.value, rhs.value);
        lhs.origin = remainder_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator&=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_and_assignment, lhs.value, rhs.value);
        lhs.origin = bitwise_and_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator|=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_or_assignment, lhs.value, rhs.value);
        lhs.origin = bitwise_or_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator^=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_xor_assignment, lhs.value, rhs.value);
        lhs.origin = bitwise_xor_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator<<=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_left_shift_assignment, lhs.value, rhs.value);
        lhs.origin = bitwise_left_shift_assignment;
        return lhs;
    }

    constexpr friend mock_type& operator>>=(mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(bitwise_right_shift_assignment, lhs.value, rhs.value);
        lhs.origin = bitwise_right_shift_assignment;
        return lhs;
    }

    // Increment and decrement operators ...............................................................................

    constexpr mock_type& operator++()
    {
        instructions->emplace_back(pre_increment, std::nullopt, value);
        origin = pre_increment;
        return *this;
    }

    constexpr mock_type& operator--()
    {
        instructions->emplace_back(pre_decrement, std::nullopt, value);
        origin = pre_decrement;
        return *this;
    }

    constexpr mock_type operator++(int)
    {
        instructions->emplace_back(post_increment, value, std::nullopt);
        return { post_increment };
    }

    constexpr mock_type operator--(int)
    {
        instructions->emplace_back(post_decrement, value, std::nullopt);
        return { post_decrement };
    }

    // Comparison operators ............................................................................................

    [[nodiscard]] constexpr friend bool operator==(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(equal_to, lhs.value, rhs.value);
        return true;
    }

    [[nodiscard]] constexpr friend bool operator!=(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(not_equal_to, lhs.value, rhs.value);
        return true;
    }

    [[nodiscard]] constexpr friend bool operator<(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(less_than, lhs.value, rhs.value);
        return true;
    }

    [[nodiscard]] constexpr friend bool operator>(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(greater_than, lhs.value, rhs.value);
        return true;
    }

    [[nodiscard]] constexpr friend bool operator<=(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(less_than_or_equal_to, lhs.value, rhs.value);
        return true;
    }

    [[nodiscard]] constexpr friend bool operator>=(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(greater_than_or_equal_to, lhs.value, rhs.value);
        return true;
    }

    [[nodiscard]] constexpr friend auto operator<=>(const mock_type& lhs, const mock_type& rhs)
    {
        lhs.instructions->emplace_back(three_way_comparison, lhs.value, rhs.value);
        return std::strong_ordering::equivalent;
    }

    // Logical operators ...............................................................................................

    [[nodiscard]] constexpr friend bool operator!(const mock_type& rhs)
    {
        rhs.instructions->emplace_back(negation, std::nullopt, rhs.value);
        return {};
    }

    // Other operators .................................................................................................

    [[nodiscard]] static constexpr mock_type operator()(const mock_type& x, int y)
    {
        x.instructions->emplace_back(static_call, x.value, y);
        return { static_call };
    }

    [[nodiscard]] constexpr mock_type operator()(int y) const
    {
        instructions->emplace_back(call, y, std::nullopt);
        return { call };
    }

    [[nodiscard]] static constexpr mock_type operator[](const mock_type& x, int y)
    {
        x.instructions->emplace_back(static_subscript, x.value, y);
        return { static_subscript };
    }

    [[nodiscard]] constexpr mock_type operator[](int y) const
    {
        instructions->emplace_back(subscript, y, std::nullopt);
        return { subscript };
    }

    // Ostream insertion ...............................................................................................

    constexpr friend std::ostream& operator<<(std::ostream& os, const mock_type& rhs)
    {
        rhs.instructions->emplace_back(ostream_insertion, os, rhs.value);
        return os;
    }

    [[nodiscard]] constexpr auto begin() const
    {
        instructions->emplace_back(begin_function, value, std::nullopt);
        return std::to_underlying(begin_function);
    }

    [[nodiscard]] constexpr auto end() const
    {
        instructions->emplace_back(end_function, value, std::nullopt);
        return std::to_underlying(end_function);
    }

    int value = 0;
    std::vector<instruction>* instructions = nullptr;
    Operation origin = noop;
};

}  // namespace stronger::tests

template <>
struct std::formatter<stronger::tests::mock_type> : std::formatter<std::string>
{
    inline static const auto
        ExpectedReturnValue = "returned from std::formatter<strongercpp::tests::mock_type>::format"s;
    auto format(const stronger::tests::mock_type& mock, std::format_context& ctx) const
    {
        using enum stronger::tests::Operation;
        using namespace std::string_literals;
        mock.instructions->emplace_back(format, mock.value, std::nullopt);
        return std::formatter<std::string>::format(ExpectedReturnValue, ctx);
    }
};

template <>
struct std::hash<stronger::tests::mock_type>
{
    constexpr hash() = default;
    static constexpr size_t ExpectedReturnValue = 68413681;
    static constexpr auto operator()(const stronger::tests::mock_type& mock) noexcept
    {
        using enum stronger::tests::Operation;
        mock.instructions->emplace_back(hash, mock.value, std::nullopt);
        return ExpectedReturnValue;
    }
};