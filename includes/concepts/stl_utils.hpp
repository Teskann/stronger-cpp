// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include <concepts>
#include <format>
#include <iosfwd>

namespace stronger::concepts
{

template <typename T>
concept supports_ostream_insertion = requires(std::ostream& os, T a) {
    { os << a } -> std::same_as<std::ostream&>;
};

template <typename T>
concept supports_std_hash = requires(const T& value) {
    { std::hash<T>{}(value) };
};

template <typename T>
concept supports_std_format = requires(const T& value) {
    { std::format("{}", value) };
};


template <typename T>
concept supports_default_construction = std::is_default_constructible_v<T>;

}  // namespace stronger::concepts