// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "test_link/api1.hpp"
#include <catch2/catch_test_macros.hpp>
namespace stronger::tests
{

TEST_CASE("Test link")
{
    // This would produce a link error if StrongInt cannot be exported
    link_test(StrongInt(10));
}

}  // namespace stronger::tests
