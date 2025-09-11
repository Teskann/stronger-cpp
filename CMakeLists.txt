cmake_minimum_required(VERSION 3.31)
project(cpp_strong_type)

# Create version.hpp ___________________________________________________________________________________________________

add_custom_target(create_version_hpp
        COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_CURRENT_SOURCE_DIR}/version.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating version.hpp"
)

# Create single header library _________________________________________________________________________________________

add_custom_target(make_header_only
        COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_CURRENT_SOURCE_DIR}/make-header-only.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating header-only library"
)

add_dependencies(make_header_only create_version_hpp)

# Library target for CPM consumers _____________________________________________________________________________________

add_library(stronger-cpp INTERFACE)
add_library(stronger-cpp::stronger-cpp ALIAS stronger-cpp)
add_dependencies(stronger-cpp make_header_only)
target_include_directories(stronger-cpp
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_compile_features(stronger-cpp INTERFACE cxx_std_23)

# Local targets ________________________________________________________________________________________________________

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 23)

    if (MSVC)
        set(WARNING_FLAGS
                /W4
                /WX
                /permissive-
                /Zc:__cplusplus
        )
    else ()
        set(WARNING_FLAGS
                -Wall
                -Wextra
                -Wpedantic
                -Werror
                -Wshadow
                -Wnon-virtual-dtor
                -Wold-style-cast
                -Wcast-align
                -Wunused
                -Woverloaded-virtual
                -Wpedantic
                -Wconversion
                -Wsign-conversion
                -Wnull-dereference
                -Wdouble-promotion
                -Wformat=2
                -Wimplicit-fallthrough
        )

        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            list(APPEND WARNING_FLAGS
                    -Wmisleading-indentation
                    -Wduplicated-cond
                    -Wduplicated-branches
                    -Wlogical-op
                    -Wuseless-cast
            )
        endif ()

        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            list(APPEND WARNING_FLAGS
                    -Wc++23-compat
                    -Wmost
            )
        endif ()
    endif ()

    add_compile_options(${WARNING_FLAGS})

    # Unit-tests target with Catch2 _______________________________________________________________________________________

    find_package(Catch2 3 REQUIRED)
    enable_testing()

    # set include path in a variable
    set(HEADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/includes)

    add_executable(stronger_cpp_tests
            tests/test_link/common.cpp
            tests/test_link/api1.cpp
            tests/test_declaration_macros.cpp
            tests/test_arithmetic.cpp
            tests/helpers.h
            tests/mock_type.hpp
            tests/test_operation_bindings.cpp
            tests/test_bool.cpp
            tests/test_types.cpp
            tests/test_strength.cpp
            tests/test_link.cpp
            tests/test_data_accessors.cpp
            includes/tag.hpp
            tests/test_tag.cpp
            tests/test_iterable.cpp
            tests/runtime_tests.cpp
            tests/test_sizes.cpp
            tests/test_make_strong.cpp
            includes/concepts/other_operators.hpp
            tests/test_options.cpp
            tests/test_version.cpp
    )
    target_compile_definitions(stronger_cpp_tests
            PRIVATE
            STRONGER_CPP_BUILD_TESTS
    )
    add_dependencies(stronger_cpp_tests make_header_only)
    target_include_directories(stronger_cpp_tests
            PRIVATE
            ${HEADERS_PATH}
    )
    target_link_libraries(stronger_cpp_tests
            PRIVATE
            Catch2::Catch2WithMain
    )

    # Download NamedType for benchmarks ____________________________________________________________________________________

    add_custom_target(download_NamedType
            COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_CURRENT_SOURCE_DIR}/tests/benchmarks/get-NamedType.py
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Getting NamedType library for benchmarks"
    )

    # Runtime benchmarks ___________________________________________________________________________________________________

    add_executable(stronger_cpp_benchmarks
            tests/benchmarks/benchmarks.cpp
    )
    add_dependencies(stronger_cpp_benchmarks make_header_only download_NamedType)
    target_include_directories(stronger_cpp_benchmarks
            PRIVATE
            ${HEADERS_PATH}
    )
    target_link_libraries(stronger_cpp_benchmarks
            PRIVATE
            Catch2::Catch2WithMain
    )

endif ()