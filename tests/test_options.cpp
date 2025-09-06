// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <stronger.hpp>
#include <type_traits>


namespace stronger::tests
{

namespace
{
using enum options;

template <typename StrongPointerOfString>
concept drilled_down = requires(StrongPointerOfString ptr) { ptr->append("Hello"); };

TEST_CASE("options")
{
    using Int = strong_type<int, tag()>;
    using IntImplicitConstruction = strong_type<int, tag(), allow_implicit_construction>;
    using IntImplicitConversion = strong_type<int, tag(), allow_implicit_conversion_to_underlying_type>;

    SECTION("No implicit conversion -> Should not be convertible")
    {
        static_assert(!std::is_convertible_v<int, Int>);
        static_assert(!std::is_convertible_v<Int, int>);
        static_assert(!std::is_convertible_v<const Int&, int>);
        static_assert(!std::is_convertible_v<const Int&, const int&>);
        static_assert(!std::is_convertible_v<Int&, int&>);
        static_assert(!std::is_convertible_v<Int&&, int&&>);
    }

    SECTION("Implicit construction -> Should be convertible from underlying to strong")
    {
        static_assert(std::is_convertible_v<int, IntImplicitConstruction>);
        static_assert(!std::is_convertible_v<IntImplicitConstruction, int>);
    }

    SECTION("Implicit conversion -> Should be convertible from strong to underlying")
    {
        static_assert(!std::is_convertible_v<int, IntImplicitConversion>);
        static_assert(std::is_convertible_v<IntImplicitConversion, int>);
        static_assert(std::is_convertible_v<IntImplicitConversion&, int&>);
        static_assert(!std::is_convertible_v<IntImplicitConversion, int&>);
        static_assert(!std::is_convertible_v<const IntImplicitConversion&, int&&>);
        static_assert(std::is_convertible_v<IntImplicitConversion&&, int&&>);
    }

    SECTION("Implicit conversion + implicit construction -> Should be convertible from underlying to strong")
    {
        using IntImplicitFull = strong_type<int, tag(), allow_implicit_construction,
                                            allow_implicit_conversion_to_underlying_type>;

        static_assert(std::is_convertible_v<int, IntImplicitFull>);
        static_assert(std::is_convertible_v<IntImplicitFull, int>);
        static_assert(std::is_convertible_v<IntImplicitFull&, int&>);
        static_assert(!std::is_convertible_v<IntImplicitFull, int&>);
        static_assert(!std::is_convertible_v<const IntImplicitFull&, int&&>);
        static_assert(std::is_convertible_v<IntImplicitFull&&, int&&>);

        SECTION("Swap option arguments should not change anything")
        {
            using IntImplicitFull2 = strong_type<int, tag(), allow_implicit_conversion_to_underlying_type,
                                                 allow_implicit_construction>;

            static_assert(std::is_convertible_v<int, IntImplicitFull2>);
            static_assert(std::is_convertible_v<IntImplicitFull2, int>);
            static_assert(std::is_convertible_v<IntImplicitFull2&, int&>);
            static_assert(!std::is_convertible_v<IntImplicitFull2, int&>);
            static_assert(!std::is_convertible_v<const IntImplicitFull2&, int&&>);
            static_assert(std::is_convertible_v<IntImplicitFull2&&, int&&>);
        }
    }

    SECTION("Drill down")
    {
        using PtrDrillDown = strong_type<std::unique_ptr<std::string>, tag(), drill_down>;
        using PtrNoDrillDown = strong_type<std::unique_ptr<std::string>, tag()>;
        static_assert(drilled_down<PtrDrillDown>);
        static_assert(!drilled_down<PtrNoDrillDown>);
        static_assert(
            []
            {
                PtrDrillDown ptr(std::make_unique<std::string>("Hello"));
                ptr->append(" world");
                return ptr->starts_with("Hello") && ptr->ends_with(" world");
            }(),
            "operator-> should be drill down and return the correct pointer");
    }
}
}  // namespace
}  // namespace stronger::tests