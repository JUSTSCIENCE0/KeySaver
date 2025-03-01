# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22.1)

project(keysaver-desktop)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(IMPLEMENTATION
    main.cpp
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(keysaver-desktop)

# compile options
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(keysaver-desktop PRIVATE
        # warnings options
        /W4 /WX
        /DUNICODE /D_UNICODE  # use unicode
        /MP                   # multiple processes build
    )
elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    target_compile_options(keysaver-desktop PRIVATE
        # warnings options
        -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
        -Wdisabled-optimization -Wformat=2 -Winit-self 
        -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual
        -Wredundant-decls -Wshadow -Wsign-promo 
        -Wstrict-overflow=5 -Wundef -Wunused
        -fdiagnostics-show-option
        -Werror
    )
endif() # GCC & Clang

# sources
target_sources(keysaver-desktop
    PRIVATE
        ${IMPLEMENTATION}
)

target_link_libraries(keysaver-desktop
    PRIVATE
        keysaver-core
        Qt6::Widgets
)
