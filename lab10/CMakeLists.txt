cmake_minimum_required(VERSION 3.16)
project(CSCI41_Lab10 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(.)

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(avl_repair_lib AVLRepairInt.cpp)
target_include_directories(avl_repair_lib PUBLIC .)

add_executable(lab10 main_demo.cpp)
target_link_libraries(lab10 PRIVATE avl_repair_lib)

add_executable(lab10_tests tests/public_tests.cpp)
target_link_libraries(lab10_tests PRIVATE avl_repair_lib)
target_include_directories(lab10_tests PRIVATE tests)

enable_testing()
add_test(NAME lab10_public COMMAND lab10_tests)
