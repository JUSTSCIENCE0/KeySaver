# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22.1)

project(keysaver-core)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
            # warnings options
            /W4 /WX
            /wd4505 /wd4100
            /DUNICODE /D_UNICODE  # use unicode
            /MP                   # multiple processes build
    )
elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
            # warnings options
            -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
            -Wdisabled-optimization -Wformat=2 -Winit-self
            -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual
            -Wredundant-decls -Wshadow -Wsign-promo
            -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros
            -Wno-parentheses -Wno-unused-parameter -fdiagnostics-show-option
            -Werror
    )
endif() # GCC & Clang

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(keysaver-core SHARED)

set(IMPLEMENTATION
    implementation.hpp
    implementation.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    target_sources(keysaver-core
        PUBLIC
            keysaver/interface.h
            keysaver/interface_android.h
        PRIVATE
            interface_android.cpp
            ${IMPLEMENTATION}
            native-lib.cpp #TODO - remove it
    )

    target_link_libraries(keysaver-core
        PRIVATE
            android
    )
endif()


