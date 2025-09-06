// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "stronger.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <array>
#include <algorithm>

namespace stronger::tests
{

TEST_CASE("Test iterable")
{
    SECTION("std::array")
    {
        using Array = strong_type<std::array<int, 3>, tag()>;
        constexpr Array a(std::array{ 1, 2, 3 });
        static_assert(std::ranges::equal(a, std::array{ 1, 2, 3 }));
        static_assert(a[1ULL] == 2);
    }

    SECTION("std::vector")
    {
        static_assert(
            []
            {
                using Vector = strong_type<std::vector<int>, tag()>;
                const Vector a(std::vector{ 1, 2, 3 });
                return std::ranges::equal(a, std::vector{ 1, 2, 3 });
            }());
    }
}

}  // namespace stronger::tests