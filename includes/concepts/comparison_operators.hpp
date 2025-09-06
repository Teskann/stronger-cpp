// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <concepts>

// Support all operators defined here:
// https://en.cppreference.com/w/cpp/language/operator_comparison.html
namespace stronger::concepts
{
template <typename T>
concept supports_equal_to = requires(T a, T b) {
    { a == b } -> std::same_as<bool>;
};

template <typename T>
concept supports_not_equal_to = requires(T a, T b) {
    { a != b } -> std::same_as<bool>;
};

template <typename T>
concept supports_less_than = requires(T a, T b) {
    { a < b } -> std::same_as<bool>;
};

template <typename T>
concept supports_greater_than = requires(T a, T b) {
    { a > b } -> std::same_as<bool>;
};

template <typename T>
concept supports_less_than_or_equal_to = requires(T a, T b) {
    { a <= b } -> std::same_as<bool>;
};

template <typename T>
concept supports_greater_than_or_equal_to = requires(T a, T b) {
    { a >= b } -> std::same_as<bool>;
};

template <typename T>
concept supports_three_way_comparison = requires(T a, T b) {
    { a <=> b };
};

}