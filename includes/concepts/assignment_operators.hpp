// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include <concepts>

// Support all operators defined here:
// https://en.cppreference.com/w/cpp/language/operator_assignment.html
// Except simple assignment
namespace stronger::concepts
{

template <typename T>
concept supports_addition_assignment = requires(T a, T b) {
    { a += b } -> std::same_as<T&>;
};

template <typename T>
concept supports_subtraction_assignment = requires(T a, T b) {
    { a -= b } -> std::same_as<T&>;
};

template <typename T>
concept supports_multiplication_assignment = requires(T a, T b) {
    { a *= b } -> std::same_as<T&>;
};

template <typename T>
concept supports_division_assignment = requires(T a, T b) {
    { a /= b } -> std::same_as<T&>;
};

template <typename T>
concept supports_remainder_assignment = requires(T a, T b) {
    { a %= b } -> std::same_as<T&>;
};

template <typename T>
concept supports_bitwise_and_assignment = requires(T a, T b) {
    { a &= b } -> std::same_as<T&>;
};

template <typename T>
concept supports_bitwise_or_assignment = requires(T a, T b) {
    { a |= b } -> std::same_as<T&>;
};

template <typename T>
concept supports_bitwise_xor_assignment = requires(T a, T b) {
    { a ^= b } -> std::same_as<T&>;
};

template <typename T>
concept supports_bitwise_left_shift_assignment = requires(T a, T b) {
    { a <<= b } -> std::same_as<T&>;
};

template <typename T>
concept supports_bitwise_right_shift_assignment = requires(T a, T b) {
    { a >>= b } -> std::same_as<T&>;
};

}  // namespace strongercpp::concepts
