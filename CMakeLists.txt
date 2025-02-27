# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22.1)

project(keysaver)

option(BUILD_KEYSAVER_TESTS "Build tests" ON)

add_subdirectory(app/src/main/cpp)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
            # warnings options
            /W4 /WX
            /wd4505
            /DUNICODE /D_UNICODE  # use unicode
            /MP                   # multiple processes build
    )
elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
            # warnings options
            -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
            -Wdisabled-optimization -Wformat=2 -Winit-self
            -Wmissing-include-dirs -Woverloaded-virtual
            -Wredundant-decls -Wshadow -Wsign-promo
            -Wstrict-overflow=5 -Wundef
            -fdiagnostics-show-option
            -Werror
    )
endif() # GCC & Clang

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (BUILD_KEYSAVER_TESTS)
    add_subdirectory(tests)
endif(BUILD_KEYSAVER_TESTS)