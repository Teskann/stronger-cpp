// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include <stronger.hpp>
#include <type_traits>

#ifndef STRONGER_CPP_VERSION_MAJOR
#error STRONGER_CPP_VERSION_MAJOR is not defined
#endif
#ifndef STRONGER_CPP_VERSION_MINOR
#error STRONGER_CPP_VERSION_MINOR is not defined
#endif
#ifndef STRONGER_CPP_VERSION_PATCH
#error STRONGER_CPP_VERSION_PATCH is not defined
#endif

namespace
{

constexpr auto Major = STRONGER_CPP_VERSION_MAJOR;
constexpr auto Minor = STRONGER_CPP_VERSION_MINOR;
constexpr auto Patch = STRONGER_CPP_VERSION_PATCH;

static_assert(std::is_same_v<std::remove_cvref_t<decltype(Major)>, int>);
static_assert(std::is_same_v<std::remove_cvref_t<decltype(Minor)>, int>);
static_assert(std::is_same_v<std::remove_cvref_t<decltype(Patch)>, int>);

}