// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "helpers.h"
#include <catch2/catch_test_macros.hpp>
#include <stronger.hpp>

namespace stronger::tests
{
namespace
{

using Int = strong_type<int, tag()>;
using String = strong_type<std::string, tag()>;

enum class Operations
{
    construct,
    copy,
    move,
    copy_assign,
    move_assign,
    destroy,
};


struct life_time
{
    using enum Operations;
    constexpr explicit life_time(std::vector<Operations>* instructions_) : instructions(instructions_)
    {
        instructions->push_back(construct);
    }
    constexpr ~life_time() { instructions->push_back(destroy); }
    constexpr life_time(const life_time& other) : instructions(other.instructions) { instructions->push_back(copy); }
    constexpr life_time(life_time&& other) noexcept : instructions(other.instructions)
    {
        instructions->push_back(move);
    }

    constexpr life_time& operator=(life_time other) noexcept
    {
        swap(*this, other);
        instructions->push_back(copy_assign);
        return *this;
    }

    constexpr life_time& operator=(life_time&& other) noexcept
    {
        swap(*this, other);
        instructions->push_back(move_assign);
        return *this;
    }

    constexpr friend void swap(life_time& first, life_time& second) noexcept
    {
        using std::swap;
        swap(first.instructions, second.instructions);
    }

    std::vector<Operations>* instructions;
};

constexpr bool test_operator_arrow_behavior()
{
    using enum Operations;
    std::vector<Operations> instructions;
    using StrongLifeTime = strong_type<life_time, tag(), options::allow_implicit_construction>;
    auto mutablevalue = StrongLifeTime(life_time(&instructions));
    const auto constValue = StrongLifeTime(life_time(&instructions));
    instructions.clear();
    decltype(auto) ptr = mutablevalue.operator->();
    static_assert(std::is_same_v<decltype(ptr), life_time*>);
    if(!instructions.empty())
        throw assertion_error("mutablevalue.operator->() triggered unexpected operations");
    instructions.clear();

    [[maybe_unused]] decltype(auto) constPtr = constValue.operator->();
    static_assert(std::is_same_v<decltype(constPtr), const life_time*>);
    if(!instructions.empty())
        throw assertion_error("constValue.operator->() triggered unexpected operations");
    instructions.clear();

    // Test that we can access the underlying object's member through the pointer
    auto* memberPtr = ptr->instructions;
    if(!instructions.empty())
        throw assertion_error("accessing member through operator-> triggered unexpected operations");

    // Verify the pointer points to the correct object
    if(memberPtr != &instructions)
        throw assertion_error("operator-> did not return pointer to the correct object");
    instructions.clear();

    using Vector = strong_type<std::vector<int>, tag()>;
    Vector v({1, 2, 3, 4, 5});
    v->emplace_back(6);
    v->emplace_back(7);
    if (!std::ranges::equal(v, std::vector({1, 2, 3, 4, 5, 6, 7})))
        throw assertion_error("v->emplace_back(6) did not work");

    const auto getList = []
    {
        return Vector({1, 2, 3, 4, 5});
    };
    if(getList()->size() != 5)
        throw assertion_error("getList()->size() != 5");

    return true;
}

constexpr bool test_operator_star_behavior()
{
    using enum Operations;
    std::vector<Operations> instructions;
    using StrongLifeTime = strong_type<life_time, tag(), options::allow_implicit_construction>;
    auto mutablevalue = StrongLifeTime(life_time(&instructions));
    const auto constValue = StrongLifeTime(life_time(&instructions));
    instructions.clear();
    auto x = *mutablevalue;
    if(instructions.back() != copy)
        throw assertion_error("x = *mutableValue did not return a copy");
    instructions.clear();

    [[maybe_unused]] decltype(auto) y = *mutablevalue;
    static_assert(std::is_same_v<decltype(y), life_time&>);
    if(!instructions.empty())
        throw assertion_error("decltype(auto) y = *mutableValue did not return a reference");
    instructions.clear();

    [[maybe_unused]] decltype(auto) yConst = *constValue;
    static_assert(std::is_same_v<decltype(yConst), const life_time&>);
    if(!instructions.empty())
        throw assertion_error("decltype(auto) yConst = *constValue did not return a const reference");
    instructions.clear();

    auto z = *std::move(mutablevalue);
    if(instructions.back() != move)
        throw assertion_error("z = *std::move(a) did not move the underlying value");
    return true;
}

constexpr bool test_implicit_conversion_behavior()
{
    using enum Operations;
    std::vector<Operations> instructions;
    using StrongLifeTime = strong_type<life_time, tag(), options::allow_implicit_construction, options::allow_implicit_conversion_to_underlying_type>;
    auto mutablevalue = StrongLifeTime(life_time(&instructions));
    const auto constValue = StrongLifeTime(life_time(&instructions));
    instructions.clear();
    life_time x = mutablevalue;
    if(instructions.back() != copy)
        throw assertion_error("life_time x = mutablevalue did not return a copy");
    instructions.clear();

    [[maybe_unused]] life_time& y = mutablevalue;
    static_assert(std::is_same_v<decltype(y), life_time&>);
    if(!instructions.empty())
        throw assertion_error("const life_time& y = mutablevalue did not return a reference");
    instructions.clear();

    [[maybe_unused]] const life_time& yConst = constValue;
    static_assert(std::is_same_v<decltype(yConst), const life_time&>);
    if(!instructions.empty())
        throw assertion_error("const life_time& yConst = constValue did not return a const reference");
    instructions.clear();

    [[maybe_unused]] life_time z = std::move(mutablevalue);
    if(instructions.back() != move)
        throw assertion_error("z = *std::move(a) did not move the underlying value");
    return true;
}

constexpr bool test_pointers()
{
    using Pointer = strong_type<std::string*, tag()>;
    std::string hello = "Hello";
    const Pointer p(&hello);
    Pointer pMutable = p;
    if(!p->starts_with("Hell"))
        throw assertion_error("!p->starts_with(\"Hell\") failed");
    if(!Pointer(&hello)->ends_with("llo"))
        throw assertion_error("!Pointer(&hello)->ends_with(\"llo\") failed");
    pMutable->append(" world");
    if(!pMutable->ends_with(" world"))
        throw assertion_error("pMutable->ends_with(\" world\")");
    return true;
}

TEST_CASE("Data accessors")
{
    SECTION("operator*")
    {
        SECTION("Return type")
        {
            static_assert(std::same_as<decltype(*std::declval<Int&>()), int&>);
            static_assert(std::same_as<decltype(*std::declval<const Int&>()), const int&>);
            static_assert(std::same_as<decltype(*std::declval<Int&&>()), int&&>);
            static_assert(std::same_as<decltype(*std::declval<const Int&&>()), const int&&>);

            static_assert(std::same_as<decltype(*std::declval<String&>()), std::string&>);
            static_assert(std::same_as<decltype(*std::declval<const String&>()), const std::string&>);
            static_assert(std::same_as<decltype(*std::declval<String&&>()), std::string&&>);
            static_assert(std::same_as<decltype(*std::declval<const String&&>()), const std::string&&>);
        }

        SECTION("Behavior")
        {
            static_assert(test_operator_star_behavior());
        }
    }

    SECTION("operator->")
    {
        static_assert(test_operator_arrow_behavior());
    }

    SECTION("implicit conversion")
    {
        static_assert(test_implicit_conversion_behavior());
    }

    SECTION("Pointers")
    {
        static_assert(test_pointers());
    }
}

}  // namespace
}  // namespace stronger::tests