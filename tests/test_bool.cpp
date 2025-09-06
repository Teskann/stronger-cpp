// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "helpers.h"
#include "stronger.hpp"

namespace stronger::tests
{

namespace
{

template <typename T>
concept supports_and = requires(T a, T b) {
    { a&& b } -> std::same_as<bool>;
};

template <typename T>
concept supports_inclusive_or = requires(T a, T b) {
    { a || b } -> std::same_as<bool>;
};

using BasicBool = strong_type<bool, tag()>;
using Bool = strong_bool<tag()>;
using Bool2 = strong_bool<tag()>;
constexpr Bool True(true);
constexpr Bool2 False(false);

constexpr bool foo(Bool, Bool2)
{
    return true;
}

template <typename T1, typename T2>
concept can_call_foo = requires(T1 a, T2 b) { foo(a, b); };

constexpr bool logical_negation_works()
{
    if constexpr(!True != false)
        throw assertion_error("!True != false");
    if constexpr(!False != true)
        throw assertion_error("!False != true");
    return true;
}

constexpr bool logical_and_is_not_implemented()
{
    if constexpr(supports_and<BasicBool>)
        throw assertion_error("Bool && Bool should not be available");
    return true;
}

constexpr bool logical_or_is_not_implemented()
{
    if constexpr(supports_inclusive_or<BasicBool>)
        throw assertion_error("Bool || Bool should not be available");
    return true;
}

constexpr bool logical_and_works_for_underlying_type()
{
    if constexpr((True && True) != true)
        throw assertion_error("True && True != true");
    if constexpr((True && False) != false)
        throw assertion_error("True && False != false");
    if constexpr((False && True) != false)
        throw assertion_error("False && True != false");
    if constexpr((False && False) != false)
        throw assertion_error("False && False != false");
    return true;
}

constexpr bool logical_or_works_for_underlying_type()
{
    if constexpr((True || True) != true)
        throw assertion_error("True || True != true");
    if constexpr((True || False) != true)
        throw assertion_error("True || False != true");
    if constexpr((False || True) != true)
        throw assertion_error("False || True != true");
    if constexpr((False || False) != false)
        throw assertion_error("False || False != false");
    return true;
}

static_assert(logical_negation_works());
static_assert(logical_and_is_not_implemented());
static_assert(logical_or_is_not_implemented());
static_assert(logical_and_works_for_underlying_type());
static_assert(logical_or_works_for_underlying_type());
static_assert(can_call_foo<Bool, Bool2>);
static_assert(!can_call_foo<bool, bool>);
static_assert(!can_call_foo<Bool2, Bool>);

}  // namespace

}  // namespace stronger::tests