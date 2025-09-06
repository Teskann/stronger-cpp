// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include <catch2/catch_test_macros.hpp>
#include <stronger.hpp>
namespace stronger::tests
{

namespace
{

TEST_CASE("Test differences")
{
    using Int1 = strong_type<int, tag()>;
    using Int2 = strong_type<int, tag()>;
    using Double = strong_type<double, tag()>;
    using String1 = strong_type<std::string, tag()>;
    using String2 = strong_type<std::string, tag()>;
    using IntExportable1 = strong_type<int, tag()>;
    using IntExportable2 = strong_type<int, tag()>;

    using Int1Implicit = strong_type<int, tag(), options::allow_implicit_construction>;
    using DoubleImplicit = strong_type<double, tag(), options::allow_implicit_construction>;
    using String1Implicit = strong_type<std::string, tag(), options::allow_implicit_construction>;

    SECTION("Two different underlying types should be different")
    {
        static_assert(!std::is_same_v<Int1, String1>);
        static_assert(!std::is_same_v<String1, Int2>);
        static_assert(!std::is_same_v<Int1, Double>);
    }

    SECTION("Two different declarations that have the same underlying type should be different")
    {
        static_assert(!std::is_same_v<Int1, Int2>);
        static_assert(!std::is_same_v<String1, String2>);
    }

    SECTION("Two exportable types should be different")
    {
        static_assert(!std::is_same_v<IntExportable1, IntExportable2>);
    }

    SECTION("Should not be convertible to underlying type")
    {
        static_assert(!std::is_convertible_v<Int1, int>);
        static_assert(!std::is_convertible_v<String1, std::string>);
        static_assert(!std::is_convertible_v<Double, double>);
    }

    SECTION("Underlying type should be convertible to strong type")
    {
        static_assert(!std::is_convertible_v<int, Int1>);
        static_assert(!std::is_convertible_v<std::string, String1>);
        static_assert(!std::is_convertible_v<double, Double>);

        SECTION("... unless user explicitly enabled implicit conversion")
        {
            static_assert(std::is_convertible_v<int, Int1Implicit>);
            static_assert(std::is_convertible_v<std::string, String1Implicit>);
            static_assert(std::is_convertible_v<double, DoubleImplicit>);
        }
    }
}

}  // namespace

}  // namespace stronger::tests