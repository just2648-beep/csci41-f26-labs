cmake_minimum_required(VERSION 3.16)
project(CSCI41_Lab08 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(lab08
    src/main.cpp
    src/quack.cpp
)

add_executable(lab08_tests
    tests/test_lab08.cpp
    src/quack.cpp
)

enable_testing()
add_test(NAME lab08_public COMMAND lab08_tests)
