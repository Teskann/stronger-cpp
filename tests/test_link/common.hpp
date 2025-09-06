// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include <stronger.hpp>

namespace stronger::tests
{

using StrongInt = strong_type<int, tag()>;

StrongInt foo(StrongInt a);

}