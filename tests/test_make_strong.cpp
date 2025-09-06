// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "helpers.h"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <stronger.hpp>
namespace stronger::tests
{

namespace
{

class Test
{
public:

    constexpr friend bool operator==(const Test&, const Test&) = default;
    constexpr Test(const int x, const std::string_view name) : m_x(x + 10), m_name(name) {}
    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;
    Test(Test&&) noexcept = delete;
    Test& operator=(Test&&) noexcept = delete;
    int m_x;
    std::string_view m_name;
};

using StrongTest = strong_type<Test, tag()>;

template <typename Callable>
using Function = strong_type<Callable, tag()>;

using Name = strong_type<std::string, tag(), options::allow_implicit_construction>;
using NameList = strong_type<std::vector<Name>, tag()>;
using namespace std::string_literals;

TEST_CASE("make_strong, make_templated_strong")
{
    SECTION("make_strong")
    {
        constexpr decltype(auto) test = make_strong<StrongTest>(1, "test");
        static_assert(std::is_same_v<std::remove_cvref_t<decltype(test)>, StrongTest>);
        static_assert(test->m_x == 11);
        static_assert(test->m_name == "test");
    }

    SECTION("make_templated_strong")
    {
        constexpr auto f = make_strong<Function>([] { return 19; });
        static_assert(f() == 19);

        static_assert([]
        {
            const auto names = stronger::make_strong<NameList>("Pierre"s, "Laurie"s, "Germain"s, "Paul"s);
            if(names[0ULL] != "Pierre"s)
                throw assertion_error("names[0ULL] != \"Pierre\"s");
            if(names[1ULL] != "Laurie"s)
                throw assertion_error("names[1ULL] != \"Laurie\"s");
            if(names[2ULL] != "Germain"s)
                throw assertion_error("names[2ULL] != \"Germain\"s");
            if(names[3ULL] != "Paul"s)
                throw assertion_error("names[3ULL] != \"Paul\"s");
            return names->size() == 4;
        }());
    }
}

}  // namespace

}  // namespace stronger::tests