// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once
#include "version.hpp"
#include "strong_type.hpp"
#include "tag.hpp"

#ifdef STRONGER_CPP_USE_DECLARATION_MACROS
#define STRONG_TYPE(Name, Type) using Name = ::stronger::strong_type<Type, ::stronger::tag()>
#endif