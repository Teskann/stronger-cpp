// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "helpers.h"
#include <catch2/catch_all.hpp>
#include <stronger.hpp>

namespace stronger::tests
{
namespace
{

template <typename T>
    requires std::is_arithmetic_v<T>
struct test_arithmetic
{
    using Arithmetic = strong_type<T, tag()>;
    static constexpr Arithmetic x{ T(10) };
    static constexpr Arithmetic y{ T(20) };
    using ref = Arithmetic::info::get_reference;
    using const_ref_like = Arithmetic::info::get_const_reference_like;
    static_assert(std::is_same_v<ref, Arithmetic&>);
    static_assert(std::is_same_v<const_ref_like, Arithmetic>);

    static_assert(std::is_same_v<typename Arithmetic::info::get_const_reference_like, Arithmetic>);
    static_assert(std::is_same_v<typename Arithmetic::info::get_const_reference_like, Arithmetic>);
    static_assert(assignment_operators_return_references<Arithmetic>());
    static_assert(assignment_operators_return_reference_to_lhs<Arithmetic>(x, y));

    static_assert(std::is_same_v<decltype(x + y), Arithmetic>);

    static_assert(x + y == Arithmetic(30));
    static constexpr bool test() { return true; }
};

TEST_CASE("Arithmetic")
{
    static_assert(test_arithmetic<int>::test());
    static_assert(test_arithmetic<unsigned int>::test());
    static_assert(test_arithmetic<long>::test());
    static_assert(test_arithmetic<long long>::test());
}
}  // namespace
}  // namespace stronger::tests