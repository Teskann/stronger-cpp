// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <ostream>
#include <type_traits>

namespace stronger::traits
{

template <typename T>
constexpr bool should_pass_by_value = std::is_trivially_copyable_v<T> && sizeof(T) <= sizeof(void*);

template <typename T>
using const_reference_like_t = std::conditional_t<should_pass_by_value<T>, T, const T&>;

}  // namespace strongercpp::traits