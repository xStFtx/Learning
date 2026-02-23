cmake_minimum_required(VERSION 3.20)
project(LinearAlg VERSION 1.0.0 LANGUAGES CXX)

# Require C++98
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Header-only library
add_library(LALib INTERFACE)
target_include_directories(LALib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Example executable
add_executable(example examples/example_usage.cpp)
target_link_libraries(example PRIVATE LALib)

# Tests
add_executable(test
    tests/main.cpp
    tests/test_addition.cpp
    tests/test_transpose.cpp
    tests/test_edge_cases.cpp
)
target_link_libraries(test PRIVATE LALib)