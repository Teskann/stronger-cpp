// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include <iostream>
#include <stronger.hpp>

int main() {
    stronger::strong_type<double, stronger::tag()> a{ 1.0 };
    std::cout << "Hello, World!" << a << std::endl;
    return 0;
}
