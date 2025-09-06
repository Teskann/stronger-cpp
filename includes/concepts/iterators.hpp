// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

namespace stronger::concepts
{

template <typename T>
concept supports_begin = requires(T a) {
    { a.begin() };
};

template <typename T>
concept supports_end = requires(T a) {
    { a.end() };
};

}  // namespace strongercpp::concepts