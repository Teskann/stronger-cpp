
// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include "./NamedType/named_type.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <random>
#include <ranges>
#include <stronger.hpp>

namespace stronger::tests::benchmarks
{

namespace
{

using NamedTypeDouble = fluent::NamedType<double, struct DoubleTag, fluent::Arithmetic>;
using StrongerDouble = strong_type<double, tag()>;

constexpr size_t Iterations = 10'000'000ULL;

template <typename T>
std::vector<T> get_random()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution dis(-1000.0, 1000.0);
    std::vector<T> result(Iterations, T(0.0));

    std::ranges::generate(result, [] { return T(dis(gen)); });
    return result;
}

template <typename T>
std::tuple<std::vector<T>, std::vector<T>, std::vector<T>> get_benchmark_params()
{
    auto a = get_random<T>();
    return std::make_tuple(a, get_random<T>(), std::move(a));
}

void benchmark(const auto& a, const auto& b, auto& result)
{
    for(auto&& [valueA, valueB, valueResult] : std::views::zip(a, b, result))
        valueResult = valueA + valueB * (valueA - valueB) / (valueA + valueB);
}

}  // namespace

TEST_CASE("Benchmark: NamedTypeDouble vs StrongerDouble Addition Performance", "[benchmark]")
{
    SECTION("double Addition")
    {
        auto [a, b, result] = get_benchmark_params<double>();
        BENCHMARK("double")
        {
            benchmark(a, b, result);
            return result;
        };
    }

    SECTION("NamedType Addition")
    {
        auto [a, b, result] = get_benchmark_params<NamedTypeDouble>();
        BENCHMARK("NamedType")
        {
            benchmark(a, b, result);
            return result;
        };
    }

    SECTION("stronger-cpp Addition")
    {
        auto [a, b, result] = get_benchmark_params<StrongerDouble>();
        BENCHMARK("stronger-cpp")
        {
            benchmark(a, b, result);
            return result;
        };
    }
}

}  // namespace stronger::tests::benchmarks