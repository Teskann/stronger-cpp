// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "stronger.hpp"
#include <catch2/catch_test_macros.hpp>
#include <print>
#include <unordered_map>
#include <array>
#include <string>
#include <sstream>
#include <vector>
#include <format>

namespace stronger::tests
{

using Name = strong_type<std::string, tag(), options::allow_implicit_construction>;
using City = strong_type<std::string, tag(), options::allow_implicit_construction>;
using Age = strong_type<int, tag()>;
using Length = strong_type<double, tag()>;
using NameList = strong_type<std::vector<Name>, tag()>;

TEST_CASE("runtime tests")
{
    SECTION("format()")
    {
        Name name("Pierre");
        City city("Paris");
        Age age(25);
        Length size(1.8);
        auto formatted = std::format("My name is {} and I live in {}. I'm {} and I am {:.2f}m tall.", name, city, age, size);
        CHECK(formatted == "My name is Pierre and I live in Paris. I'm 25 and I am 1.80m tall.");

        auto fullName = name + Name(" Dupont");
        CHECK(fullName == Name("Pierre Dupont"));
    }

    SECTION("having fun")
    {
        NameList names;
        names->emplace_back("Pierre");
        names->emplace_back("Laurie");
        names->emplace_back("Germain");

        std::ostringstream oss;
        for(const auto& name : names)
            oss << name << " ";
        CHECK(oss.str() == "Pierre Laurie Germain ");
        CHECK(names[0ULL] == Name("Pierre"));
        CHECK(names[1ULL] == Name("Laurie"));
    }

    SECTION("hash() -> Use in std::unordered_map")
    {
        const std::unordered_map<Name, int> map = {
            {Name("Pierre"), 1},
            {Name("Laurie"), 2},
            {Name("Germain"), 3},
        };
        CHECK(map.size() == 3);
        CHECK(map.at(Name("Pierre")) == 1);
        CHECK(map.at(Name("Laurie")) == 2);
        CHECK(map.at(Name("Germain")) == 3);
    }
}

}