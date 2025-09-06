// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <concepts>

// Support all operators defined here:
// https://en.cppreference.com/w/cpp/language/operator_arithmetic.html
namespace stronger::concepts
{

template <typename T>
concept supports_unary_plus = requires(T a) {
    { +a } -> std::same_as<T>;
};

template <typename T>
concept supports_unary_minus = requires(T a) {
    { -a } -> std::same_as<T>;
};

template <typename T>
concept supports_addition = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template <typename T>
concept supports_subtraction = requires(T a, T b) {
    { a - b } -> std::same_as<T>;
};

template <typename T>
concept supports_multiplication = requires(T a, T b) {
    { a* b } -> std::same_as<T>;
};

template <typename T>
concept supports_division = requires(T a, T b) {
    { a / b } -> std::same_as<T>;
};

template <typename T>
concept supports_remainder = requires(T a, T b) {
    { a % b } -> std::same_as<T>;
};

template <typename T>
concept supports_bitwise_not = requires(T a) {
    { ~a } -> std::same_as<T>;
};

template <typename T>
concept supports_bitwise_and = requires(T a, T b) {
    { a& b } -> std::same_as<T>;
};

template <typename T>
concept supports_bitwise_or = requires(T a, T b) {
    { a | b } -> std::same_as<T>;
};

template <typename T>
concept supports_bitwise_xor = requires(T a, T b) {
    { a ^ b } -> std::same_as<T>;
};

template <typename T>
concept supports_bitwise_left_shift = requires(T a, T b) {
    { a << b } -> std::same_as<T>;
};

template <typename T>
concept supports_bitwise_right_shift = requires(T a, T b) {
    { a >> b } -> std::same_as<T>;
};

}  // namespace strongercpp::concepts
