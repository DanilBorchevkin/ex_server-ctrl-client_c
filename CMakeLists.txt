###############################################################################
# CMake header
###############################################################################

cmake_minimum_required(VERSION 3.14)

###############################################################################
# Project
###############################################################################

project(AwesomeProject)

###############################################################################
# Options
###############################################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

###############################################################################
# Libraries
###############################################################################

add_library(
    module SHARED
    src/module.c
    inc/module.h
)

target_include_directories(
    module PUBLIC
    inc/
    src/
)

###############################################################################
# Executable
###############################################################################

add_executable(
    AwesomeProject
    src/main.c
)

target_include_directories(
    AwesomeProject PUBLIC
    "${PROJECT_BINARY_DIR}"
    inc/
)

target_link_libraries(
    AwesomeProject
    module
)

###############################################################################
# Tests
###############################################################################

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    test_unit_base
    ./test/unit_c/test_unit_base.cpp
)

target_include_directories(
    test_unit_base PUBLIC
    "${PROJECT_BINARY_DIR}"
    inc/
    src/
)

target_link_libraries(
    test_unit_base
    GTest::gtest_main
    module
)

add_executable(
    test_int_base
    ./test/integration_c/test_int_base.cpp
)

target_include_directories(
    test_int_base PUBLIC
    "${PROJECT_BINARY_DIR}"
    inc/
    src/
)

target_link_libraries(
    test_int_base
    GTest::gtest_main
    module
)

include(GoogleTest)
gtest_discover_tests(test_unit_base)
gtest_discover_tests(test_int_base)

###############################################################################
# End of CMake file
###############################################################################
