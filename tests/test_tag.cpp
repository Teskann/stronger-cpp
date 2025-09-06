// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "helpers.h"
#include "stronger.hpp"
#include <catch2/catch_test_macros.hpp>
namespace stronger::internal
{
struct test_tag_utils : tag_utils
{
    using tag_utils::hash_location;
    using tag_utils::get_filename_only;
    using tag_utils::hash_combine;
    using tag_utils::hash_string;
};
}  // namespace stronger::internal

namespace stronger::tests
{
using namespace stronger::internal;

constexpr bool test_get_filename_only(const char* location, std::string_view expected)
{
    const std::string_view filename_only = test_tag_utils::get_filename_only(location);
    return filename_only == expected;
}

TEST_CASE("linkable")
{
    SECTION("get_filename_only")
    {
        using namespace std::string_view_literals;
        SECTION("Current location")
        {
            constexpr auto location = std::source_location::current().file_name();
            static_assert(test_get_filename_only(location, "test_tag.cpp"sv));
        }

        SECTION("Custom")
        {
            static constexpr auto location = "/home/test/long/long/long/path/this/is/the/file.cpp";
            static_assert(test_get_filename_only(location, "file.cpp"sv));
        }

        SECTION("Unicode")
        {
            static constexpr auto location = "/home/unicode/étrange/œchik/test.h";
            static_assert(test_get_filename_only(location, "test.h"sv));
        }

        SECTION("Windows")
        {
            static constexpr auto location = R"(C:\Users\some user\some folder\file.cpp)";
            static_assert(test_get_filename_only(location, "file.cpp"sv));
        }
    }

    SECTION("hash_string")
    {
        static_assert(test_tag_utils::hash_string("hello") != test_tag_utils::hash_string("world"));
        static_assert(test_tag_utils::hash_string("0") != test_tag_utils::hash_string(""));
        static_assert(test_tag_utils::hash_string("yes") != test_tag_utils::hash_string("no"));
    }

    SECTION("hash_location")
    {
        // clang-format off
        static_assert(test_tag_utils::hash_location(std::source_location::current()) !=
                      test_tag_utils::hash_location(std::source_location::current()));
        static_assert(test_tag_utils::hash_location(std::source_location::current()) != test_tag_utils::hash_location(std::source_location::current()));
        // clang-format on
        constexpr std::source_location location = std::source_location::current();
        static_assert(test_tag_utils::hash_location(location) !=
                      test_tag_utils::hash_location(std::source_location::current()));
        static_assert(test_tag_utils::hash_location(location) == test_tag_utils::hash_location(location));
    }

    SECTION("tag()")
    {
        static_assert(tag() != tag());
        static_assert(tag("hello") != tag("world"));
        constexpr auto tag1 = tag();
        static_assert(tag1 != tag());
        static_assert(tag1 == tag1);
        constexpr auto tag2 = tag("hello");
        constexpr auto tag3 = tag("hello");
        static_assert(tag2 == tag3);

    }
}

}  // namespace stronger::tests