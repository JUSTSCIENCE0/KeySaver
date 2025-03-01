# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22.1)

project(keysaver)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(ProcessorCount)
ProcessorCount(NUM_CORES)

if(NUM_CORES GREATER 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${NUM_CORES} CACHE STRING "Set parallel build level")
endif()

option(BUILD_KEYSAVER_DESKTOP "Build desktop application" ON)
option(BUILD_KEYSAVER_TESTS   "Build tests"               ON)

add_subdirectory(app/src/main/cpp)

if (BUILD_KEYSAVER_DESKTOP)
    add_subdirectory(desktop)
endif(BUILD_KEYSAVER_DESKTOP)

if (BUILD_KEYSAVER_TESTS)
    add_subdirectory(tests)
endif(BUILD_KEYSAVER_TESTS)