// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <stronger.hpp>
#include <vector>
namespace stronger::tests
{

namespace
{

TEST_CASE("sizeof")
{
    using Int = strong_type<int, tag()>;
    using Double = strong_type<double, tag()>;
    using Vector = strong_type<std::vector<int>, tag()>;
    using String = strong_type<std::string, tag()>;

    static_assert(sizeof(Int) == sizeof(int));
    static_assert(sizeof(Double) == sizeof(double));
    static_assert(sizeof(Vector) == sizeof(std::vector<int>));
    static_assert(sizeof(String) == sizeof(std::string));
}
}  // namespace
}  // namespace stronger::tests