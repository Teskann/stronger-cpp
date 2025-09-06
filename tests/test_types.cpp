// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "stronger.hpp"
#include <catch2/catch_test_macros.hpp>
namespace stronger::tests
{

namespace
{

struct small
{
    char a;
    char b;
};

struct large
{
    double x;
    double y;
};

TEST_CASE("Test types")
{
    using Int = strong_type<int, tag()>;
    using Double = strong_type<double, tag()>;
    using Ptr = strong_type<int*, tag()>;
    using Str = strong_type<std::string, tag()>;
    using Vector = strong_type<std::vector<int>, tag()>;
    using UniquePtr = strong_type<std::unique_ptr<int>, tag()>;

    using Small = strong_type<small, tag()>;
    using Large = strong_type<large, tag()>;

    SECTION("underlying_type")
    {
        static_assert(std::is_same_v<Int::underlying_type, int>);
        static_assert(std::is_same_v<Double::underlying_type, double>);
        static_assert(std::is_same_v<Ptr::underlying_type, int*>);
        static_assert(std::is_same_v<Str::underlying_type, std::string>);
        static_assert(std::is_same_v<Vector::underlying_type, std::vector<int>>);
        static_assert(std::is_same_v<UniquePtr::underlying_type, std::unique_ptr<int>>);
        static_assert(std::is_same_v<Small::underlying_type, small>);
        static_assert(std::is_same_v<Large::underlying_type, large>);
    }

    SECTION("const_reference_like")
    {
        SECTION("Small types -> const_reference_like should be a copy")
        {
            static_assert(std::is_same_v<Int::info::get_const_reference_like, Int>);
            static_assert(std::is_same_v<Double::info::get_const_reference_like, Double>);
            static_assert(std::is_same_v<Ptr::info::get_const_reference_like, Ptr>);
            static_assert(std::is_same_v<Small::info::get_const_reference_like, Small>);
        }

        SECTION("Large types or non trivially copyable -> const_reference_like should be a const reference")
        {
            static_assert(std::is_same_v<Large::info::get_const_reference_like, const Large&>);
            static_assert(std::is_same_v<Str::info::get_const_reference_like, const Str&>);
            static_assert(std::is_same_v<Vector::info::get_const_reference_like, const Vector&>);
            static_assert(std::is_same_v<UniquePtr::info::get_const_reference_like, const UniquePtr&>);
        }
    }

    SECTION("reference")
    {
        static_assert(std::is_same_v<Int::info::get_reference, Int&>);
        static_assert(std::is_same_v<Double::info::get_reference, Double&>);
        static_assert(std::is_same_v<Ptr::info::get_reference, Ptr&>);
        static_assert(std::is_same_v<Small::info::get_reference, Small&>);
        static_assert(std::is_same_v<Large::info::get_reference, Large&>);
        static_assert(std::is_same_v<Str::info::get_reference, Str&>);
        static_assert(std::is_same_v<Vector::info::get_reference, Vector&>);
        static_assert(std::is_same_v<UniquePtr::info::get_reference, UniquePtr&>);
    }
}

}  // namespace

}  // namespace stronger::tests