// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

namespace stronger::concepts
{

template <typename T, typename... Args>
concept supports_static_call = requires(Args... args) {
    { T::operator()(args...) };
};

template <typename T, typename... Args>
concept supports_non_static_call = requires(T a, Args... args) {
    { a.operator()(args...) };
} && !supports_static_call<T, Args...>;

template <typename T, typename... Args>
concept supports_static_subscript = requires(Args... args) {
    { T::operator[](args...) };
};

template <typename T, typename... Args>
concept supports_non_static_subscript = requires(T a, Args... args) {
    { a[args...] };
} && !supports_static_subscript<T, Args...>;

}  // namespace stronger::concepts
