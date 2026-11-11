cmake_minimum_required(VERSION 3.16)
project(CSCI41_Lab09 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(lab09
        src/main.cpp
        src/wordcount.cpp
)

add_executable(lab09_tests
        tests/test_lab09.cpp
        src/wordcount.cpp
)

enable_testing()
add_test(NAME lab09_public COMMAND lab09_tests)
