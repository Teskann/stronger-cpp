// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include <concepts>

// Support operators defined here:
// https://en.cppreference.com/w/cpp/language/operator_logical.html
// Exclude && and || because
namespace stronger::concepts
{

template <typename T>
concept supports_negation = requires(T a) {
    { !a } -> std::same_as<bool>;
};

}  // namespace strongercpp::concepts