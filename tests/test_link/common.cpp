// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "common.hpp"

namespace stronger::tests
{

StrongInt foo(const StrongInt a)
{
    return a + StrongInt(10);
}

}