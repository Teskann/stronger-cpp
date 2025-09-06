// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "helpers.h"
#include "mock_type.hpp"
#include "stronger.hpp"
#include <unordered_set>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <string>
#include <format>

namespace stronger::tests
{

namespace
{

using Strong = strong_type<mock_type, tag()>;
using enum Operation;

constexpr void test_arithmetic_operators_bindings(const Strong& lhs, const Strong& rhs, auto* instructions)
{
    Strong result = +rhs;
    if(instructions->back() != instruction(unary_plus, std::nullopt, rhs->value))
        throw assertion_error("+rhs failed");
    if(result->origin != unary_plus)
        throw assertion_error("result->origin != unary_plus");

    result = -rhs;
    if(instructions->back() != instruction(unary_minus, std::nullopt, rhs->value))
        throw assertion_error("-rhs failed");
    if(result->origin != unary_minus)
        throw assertion_error("result->origin != unary_minus");

    result = lhs + rhs;
    if(instructions->back() != instruction(addition, lhs->value, rhs->value))
        throw assertion_error("lhs + rhs failed");
    if(result->origin != addition)
        throw assertion_error("result->origin != addition");

    result = lhs - rhs;
    if(instructions->back() != instruction(subtraction, lhs->value, rhs->value))
        throw assertion_error("lhs - rhs failed");
    if(result->origin != subtraction)
        throw assertion_error("result->origin != subtraction");

    result = lhs * rhs;
    if(instructions->back() != instruction(multiplication, lhs->value, rhs->value))
        throw assertion_error("lhs * rhs failed");
    if(result->origin != multiplication)
        throw assertion_error("result->origin != multiplication");

    result = lhs / rhs;
    if(instructions->back() != instruction(division, lhs->value, rhs->value))
        throw assertion_error("lhs / rhs failed");
    if(result->origin != division)
        throw assertion_error("result->origin != division");

    result = lhs % rhs;
    if(instructions->back() != instruction(remainder, lhs->value, rhs->value))
        throw assertion_error("lhs % rhs failed");
    if(result->origin != remainder)
        throw assertion_error("result->origin != remainder");

    result = ~rhs;
    if(instructions->back() != instruction(bitwise_not, std::nullopt, rhs->value))
        throw assertion_error("~rhs failed");
    if(result->origin != bitwise_not)
        throw assertion_error("result->origin != bitwise_not");

    result = lhs & rhs;
    if(instructions->back() != instruction(bitwise_and, lhs->value, rhs->value))
        throw assertion_error("lhs & rhs failed");
    if(result->origin != bitwise_and)
        throw assertion_error("result->origin != bitwise_and");

    result = lhs | rhs;
    if(instructions->back() != instruction(bitwise_or, lhs->value, rhs->value))
        throw assertion_error("lhs | rhs failed");
    if(result->origin != bitwise_or)
        throw assertion_error("result->origin != bitwise_or");

    result = lhs ^ rhs;
    if(instructions->back() != instruction(bitwise_xor, lhs->value, rhs->value))
        throw assertion_error("lhs ^ rhs failed");
    if(result->origin != bitwise_xor)
        throw assertion_error("result->origin != bitwise_xor");

    result = lhs << rhs;
    if(instructions->back() != instruction(bitwise_left_shift, lhs->value, rhs->value))
        throw assertion_error("lhs << rhs failed");
    if(result->origin != bitwise_left_shift)
        throw assertion_error("result->origin != bitwise_left_shift");

    result = lhs >> rhs;
    if(instructions->back() != instruction(bitwise_right_shift, lhs->value, rhs->value))
        throw assertion_error("lhs >> rhs failed");
    if(result->origin != bitwise_right_shift)
        throw assertion_error("result->origin != bitwise_right_shift");
}

constexpr void test_assignment_operations_bindings(Strong& lhs, const Strong& rhs, auto* instructions)
{
    Strong result = lhs += rhs;
    if(instructions->back() != instruction(addition_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs += rhs failed");
    if(result->origin != addition_assignment)
        throw assertion_error("result->origin != addition_assignment");

    result = lhs -= rhs;
    if(instructions->back() != instruction(subtraction_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs -= rhs failed");
    if(result->origin != subtraction_assignment)
        throw assertion_error("result->origin != subtraction_assignment");

    result = lhs *= rhs;
    if(instructions->back() != instruction(multiplication_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs *= rhs failed");
    if(result->origin != multiplication_assignment)
        throw assertion_error("result->origin != multiplication_assignment");

    result = lhs /= rhs;
    if(instructions->back() != instruction(division_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs /= rhs failed");
    if(result->origin != division_assignment)
        throw assertion_error("result->origin != division_assignment");

    result = lhs %= rhs;
    if(instructions->back() != instruction(remainder_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs %= rhs failed");
    if(result->origin != remainder_assignment)
        throw assertion_error("result->origin != remainder_assignment");

    result = lhs &= rhs;
    if(instructions->back() != instruction(bitwise_and_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs &= rhs failed");
    if(result->origin != bitwise_and_assignment)
        throw assertion_error("result->origin != bitwise_and_assignment");

    result = lhs |= rhs;
    if(instructions->back() != instruction(bitwise_or_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs |= rhs failed");
    if(result->origin != bitwise_or_assignment)
        throw assertion_error("result->origin != bitwise_or_assignment");

    result = lhs ^= rhs;
    if(instructions->back() != instruction(bitwise_xor_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs ^= rhs failed");
    if(result->origin != bitwise_xor_assignment)
        throw assertion_error("result->origin != bitwise_xor_assignment");

    result = lhs <<= rhs;
    if(instructions->back() != instruction(bitwise_left_shift_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs <<= rhs failed");
    if(result->origin != bitwise_left_shift_assignment)
        throw assertion_error("result->origin != bitwise_left_shift_assignment");

    result = lhs >>= rhs;
    if(instructions->back() != instruction(bitwise_right_shift_assignment, lhs->value, rhs->value))
        throw assertion_error("lhs >>= rhs failed");
    if(result->origin != bitwise_right_shift_assignment)
        throw assertion_error("result->origin != bitwise_right_shift_assignment");
}

constexpr void test_comparison_operators_bindings(const Strong& lhs, const Strong& rhs, auto* instructions)
{
    std::ignore = lhs == rhs;
    if(instructions->back() != instruction(equal_to, lhs->value, rhs->value))
        throw assertion_error("lhs == rhs failed");

    std::ignore = lhs != rhs;
    if(instructions->back() != instruction(not_equal_to, lhs->value, rhs->value))
        throw assertion_error("lhs != rhs failed");

    std::ignore = lhs < rhs;
    if(instructions->back() != instruction(less_than, lhs->value, rhs->value))
        throw assertion_error("lhs < rhs failed");

    std::ignore = lhs > rhs;
    if(instructions->back() != instruction(greater_than, lhs->value, rhs->value))
        throw assertion_error("lhs > rhs failed");

    std::ignore = lhs <= rhs;
    if(instructions->back() != instruction(less_than_or_equal_to, lhs->value, rhs->value))
        throw assertion_error("lhs <= rhs failed");

    std::ignore = lhs >= rhs;
    if(instructions->back() != instruction(greater_than_or_equal_to, lhs->value, rhs->value))
        throw assertion_error("lhs >= rhs failed");

    std::ignore = lhs <=> rhs;
    if(instructions->back() != instruction(three_way_comparison, lhs->value, rhs->value))
        throw assertion_error("lhs >= rhs failed");
}

constexpr void test_increment_and_decrement_operators_bindings(Strong& lhs, Strong& rhs, auto* instructions)
{
    Strong result = ++rhs;
    if(instructions->back() != instruction(pre_increment, std::nullopt, rhs->value))
        throw assertion_error("++rhs failed");

    result = --rhs;
    if(instructions->back() != instruction(pre_decrement, std::nullopt, rhs->value))
        throw assertion_error("--rhs failed");

    result = lhs++;
    if(instructions->back() != instruction(post_increment, lhs->value, std::nullopt))
        throw assertion_error("lhs++ failed");

    result = lhs--;
    if(instructions->back() != instruction(post_decrement, lhs->value, std::nullopt))
        throw assertion_error("lhs-- failed");
}

constexpr void test_logical_operators_bindings(const Strong& rhs, auto* instructions)
{
    std::ignore = !rhs;
    if(instructions->back() != instruction(negation, std::nullopt, rhs->value))
        throw assertion_error("!rhs failed");
}

constexpr void test_other_operators_bindings(const Strong& lhs, auto* instructions)
{
    decltype(auto) result = Strong::operator()(*lhs, 10);
    if(instructions->back() != instruction(static_call, lhs->value, 10))
        throw assertion_error("Strong::operator()(lhs, 10) failed");
    if(result.origin != static_call)
        throw assertion_error("result->origin != static_call");

    result = lhs(-25);
    if(instructions->back() != instruction(call, -25, std::nullopt))
        throw assertion_error("lhs(-25) failed");
    if(result.origin != call)
        throw assertion_error("result->origin != static_call");

    result = Strong::operator[](*lhs, 10);
    if(instructions->back() != instruction(static_subscript, lhs->value, 10))
        throw assertion_error("Strong::operator[](lhs, 10) failed");
    if(result.origin != static_subscript)
        throw assertion_error("result->origin != static_subscript");

    result = lhs[79];
    if(instructions->back() != instruction(subscript, 79, std::nullopt))
        throw assertion_error("lhs[79] failed");
    if(result.origin != subscript)
        throw assertion_error("result->origin != subscript");
}

constexpr void test_begin_end_bindings(const Strong& lhs, auto* instructions)
{
    decltype(auto) result = lhs.begin();
    if(instructions->back() != instruction(begin_function, lhs->value, std::nullopt))
        throw assertion_error("lhs.begin() failed");
    if(result != std::to_underlying(begin_function))
        throw assertion_error("result != std::to_underlying(begin_function)");

    result = lhs.end();
    if(instructions->back() != instruction(end_function, lhs->value, std::nullopt))
        throw assertion_error("lhs.end() failed");
    if(result != std::to_underlying(end_function))
        throw assertion_error("result != std::to_underlying(end_function)");
}

constexpr void test_hash(const Strong& lhs, auto* instructions)
{
    const auto result = std::hash<Strong>{}(lhs);
    if(instructions->back() != instruction(hash, lhs->value, std::nullopt))
        throw assertion_error("std::hash<Strong>{}(lhs) failed");
    if(result != std::hash<Strong::underlying_type>::ExpectedReturnValue)
        throw assertion_error("std::hash<Strong>{}(lhs) returned wrong value");
}

constexpr bool test_operation_bindings()
{
    std::vector<instruction> instructions;
    constexpr int lhsValue = 221;
    constexpr int rhsValue = 123;
    auto lhs = Strong({ lhsValue, &instructions });
    auto rhs = Strong({ rhsValue, &instructions });
    test_arithmetic_operators_bindings(lhs, rhs, &instructions);
    test_assignment_operations_bindings(lhs, rhs, &instructions);
    test_comparison_operators_bindings(lhs, rhs, &instructions);
    test_increment_and_decrement_operators_bindings(lhs, rhs, &instructions);
    test_logical_operators_bindings(rhs, &instructions);
    test_other_operators_bindings(lhs, &instructions);
    test_begin_end_bindings(lhs, &instructions);
    test_hash(lhs, &instructions);
    return true;
}

static_assert(test_operation_bindings());

TEST_CASE("Operation bindings")
{
    SECTION("Test operation bindings. Already done at compile time, let here for debugging purposes")
    {
        REQUIRE(test_operation_bindings());
    }

    SECTION("What could not be tested at compile time")
    {
        std::vector<instruction> instructions;
        const auto mock = Strong({ 123, &instructions });
        SECTION("ostream insertion")
        {
            std::ostringstream oss;
            oss << mock;
            REQUIRE(instructions.back() == instruction(ostream_insertion, oss, mock->value));
        }

        SECTION("Format")
        {
            using namespace std::string_literals;
            const auto result = std::format("{}", mock);
            REQUIRE(instructions.back() == instruction(format, mock->value, std::nullopt));
            REQUIRE(result == std::formatter<mock_type>::ExpectedReturnValue);
        }
    }
}

}  // namespace

}  // namespace stronger::tests