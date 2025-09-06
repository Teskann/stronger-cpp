// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#define STRONGER_CPP_USE_DECLARATION_MACROS
#include <stronger.hpp>
#undef STRONGER_CPP_USE_DECLARATION_MACROS

namespace stronger::tests
{
namespace
{

STRONG_TYPE(Distance, double);
STRONG_TYPE(Distance2, double);

static_assert(!std::is_same_v<Distance, Distance2>);

constexpr Distance d1{ 10.0 };
constexpr Distance2 d2{ 10.0 };
static_assert(*d1 == 10);
static_assert(*d2 == 10);


}  // namespace
}  // namespace stronger::tests