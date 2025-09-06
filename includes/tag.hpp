// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include <source_location>
#include <string_view>

namespace stronger
{

namespace internal
{

struct tag_utils
{
    static consteval std::size_t hash_location(const std::source_location location)
    {
        std::size_t hash = location.line();
        hash = hash_combine(hash, location.column());
        hash = hash_combine(hash, hash_string(location.function_name()));
        hash = hash_combine(hash, hash_string(get_filename_only(location.file_name())));
        return hash;
    }

    static constexpr std::size_t hash_string(const char* str)
    {
        // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
        static constexpr std::size_t fnv_offset_basis = 0xcbf29ce484222325;
        static constexpr std::size_t fnv_prime = 0x100000001b3;

        std::size_t hash = fnv_offset_basis;

        while(*str != '\0')
        {
            hash ^= static_cast<std::size_t>(*str);
            hash *= fnv_prime;
            ++str;
        }

        return hash;
    }

private:

    static constexpr const char* get_filename_only(const char* path)
    {
        const std::string_view path_view(path);
        const size_t pos = path_view.find_last_of("/\\");
        return pos == std::string_view::npos ? path : path + pos + 1;
    }

    static constexpr std::size_t hash_combine(const std::size_t seed, const std::size_t hash_value)
    {
        // Based on boost::hash_combine
        return seed ^ (hash_value + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }

    #ifdef STRONGER_CPP_BUILD_TESTS
    friend struct test_tag_utils;
    #endif
};

}  // namespace internal

/**
 * Utility function to make a strong_type unique in several translation units
 * @param location Leave this argument as default
 * @return
 */
consteval std::size_t tag(const std::source_location location = std::source_location::current())
{
    return internal::tag_utils::hash_location(location);
}

consteval std::size_t tag(const char* text)
{
    return internal::tag_utils::hash_string(text);
}

}  // namespace stronger
