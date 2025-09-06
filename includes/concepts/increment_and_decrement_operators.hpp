// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include <concepts>

namespace stronger::concepts
{

template <typename T>
concept supports_pre_increment = requires(T a) {
    { ++a } -> std::same_as<T&>;
};

template <typename T>
concept supports_pre_decrement = requires(T a) {
    { --a } -> std::same_as<T&>;
};

template <typename T>
concept supports_post_increment = requires(T a) {
    { a++ } -> std::same_as<T>;
};

template <typename T>
concept supports_post_decrement = requires(T a) {
    { a-- } -> std::same_as<T>;
};

}  // namespace strongercpp::concepts
