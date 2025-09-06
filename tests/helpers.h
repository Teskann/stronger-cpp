// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

namespace stronger::tests
{

class assertion_error final : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

template <typename StrongType>
consteval bool assignment_operators_return_references()
{
    using ref = StrongType::info::get_reference;
    [[maybe_unused]] StrongType a;
    [[maybe_unused]] StrongType b;
    static_assert(std::is_same_v<decltype(a += b), ref>);
    static_assert(std::is_same_v<decltype(a -= b), ref>);
    static_assert(std::is_same_v<decltype(a *= b), ref>);
    static_assert(std::is_same_v<decltype(a /= b), ref>);
    static_assert(std::is_same_v<decltype(a %= b), ref>);
    static_assert(std::is_same_v<decltype(a &= b), ref>);
    static_assert(std::is_same_v<decltype(a |= b), ref>);
    static_assert(std::is_same_v<decltype(a ^= b), ref>);
    return true;
}

template <typename StrongType>
consteval bool assignment_operators_return_reference_to_lhs(StrongType lhs, StrongType rhs)
{
    auto pa = &lhs;

    std::vector<StrongType*> assignmentResults = { &(lhs += rhs),  &(lhs -= rhs), &(lhs *= rhs), &(lhs /= rhs),
                                                   &(lhs %= rhs),  &(lhs &= rhs), &(lhs |= rhs), &(lhs ^= rhs),
                                                   &(lhs <<= rhs), &(lhs >>= rhs) };
    const bool result = std::ranges::all_of(assignmentResults, [pa](auto& p) { return p == pa; });
    return result;
}

}  // namespace stronger::tests
