// Copyright (c) 2025 Clément Metz
// Licensed under the MIT License. See LICENSE file for details.

#include <string>
#include <vector>
#ifdef COMPILE_TIME_BENCHMARK_STRONGER
    #include "../../public/stronger.hpp"
    #define STRONG_TYPE(Name, Type)  using Name = ::stronger::strong_type<Type, ::stronger::tag()>
    #define MAKE_STRONG(Type, Value) ::stronger::make_strong<Type>(Value)
    #define ACCESS                   ->
#elifdef COMPILE_TIME_BENCHMARK_NAMED_TYPE
    #include "./NamedType/named_type.hpp"
    #define STRONG_TYPE(Name, Type)                                                                        \
        using Name = ::fluent::NamedType<Type, struct Name##Tag, ::fluent::Arithmetic, ::fluent::Callable, \
                                         ::fluent::Dereferencable>;
    #define MAKE_STRONG(Type, Value) ::fluent::make_named<Type>(Value)
    #define ACCESS                   ->
#else
    #define STRONG_TYPE(Name, Type)  using Name = Type;
    #define MAKE_STRONG(Type, Value) Value
    #define ACCESS                   .
#endif

#ifndef INCLUDE_ONLY

STRONG_TYPE(Int, int);
STRONG_TYPE(Double1, double);
STRONG_TYPE(Double2, double);
STRONG_TYPE(Double3, double);
STRONG_TYPE(Double4, double);
STRONG_TYPE(Double5, double);
STRONG_TYPE(Double6, double);
STRONG_TYPE(Double7, double);
STRONG_TYPE(Double8, double);
STRONG_TYPE(Double9, double);
STRONG_TYPE(Double10, double);
STRONG_TYPE(String, std::string);
STRONG_TYPE(String2, std::string);
STRONG_TYPE(String3, std::string);
STRONG_TYPE(VectorInt, std::vector<int>);
STRONG_TYPE(VectorDouble, std::vector<double>);
STRONG_TYPE(VectorString, std::vector<String>);

String concat(String a, String b)
{
    return a + b;
}

Double1 foo(Double1 a, Double1 b, Double1 c, Double1 d)
{
    return a + b + c - d;
}

Int foo(Int a, Int b, Int c, Int d)
{
    return (a + b - c - d % a) << (a + b) >> (c - d);
}


template <typename Callable>
STRONG_TYPE(Function, Callable);

int main()
{
    auto f = MAKE_STRONG(Function, [](Double1 a) { return a + Double1(1); });
    auto f2 = MAKE_STRONG(Function, [](const VectorString& a) { return a ACCESS size(); });
    auto f3 = MAKE_STRONG(Function, ([](const VectorString& a, Double10, Double2, Double4, VectorDouble&)
                                     { return a ACCESS size(); }));
    Int a(-78);
    Int b(12);
    Int c(987);
    Int d(-73463);
    Double1 d1(1.0);
    Double2 d2(1.0);
    Double3 d3(1.0);
    String s("hello");
    String2 s2("world");
    String3 s3("!");
    VectorInt vi{
        std::vector{ 1, 2, 3, 4, 5 }
    };
    VectorDouble vd{
        std::vector{ 1.0, 2.0, 3.0, 4.0, 5.0 }
    };
    VectorString vs{
        std::vector{ String{ "hello" }, String{ "world" }, String{ "!" } }
    };

    std::ignore = foo(d1, d1, d1, d1);
    std::ignore = foo(a, b, c, d);
    std::ignore = f;
    std::ignore = f2;
    std::ignore = f3;
    std::ignore = d1;
    std::ignore = d2;
    std::ignore = d3;
    std::ignore = s;
    std::ignore = s2;
    std::ignore = s3;
    std::ignore = vi;
    std::ignore = vd;
    std::ignore = vs;
}
#else
int main()
{
}
#endif