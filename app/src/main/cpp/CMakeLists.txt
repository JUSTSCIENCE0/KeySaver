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
            -Wmissing-include-dirs -Woverloaded-virtual
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
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(OPENSSL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/android_arm64-v8a/openssl/include)
        set(OPENSSL_LIBRARIES ${CMAKE_SOURCE_DIR}/external/android_arm64-v8a/openssl/lib/libcrypto.a
                              ${CMAKE_SOURCE_DIR}/external/android_arm64-v8a/openssl/lib/libssl.a)
        set(PROTOBUF_LIBRARIES
                ${CMAKE_SOURCE_DIR}/external/android_arm64-v8a/protobuf/lib/libprotobuf.a)
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        set(OPENSSL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/android_x86_64/openssl/include)
        set(OPENSSL_LIBRARIES ${CMAKE_SOURCE_DIR}/external/android_x86_64/openssl/lib/libcrypto.a
                              ${CMAKE_SOURCE_DIR}/external/android_x86_64/openssl/lib/libssl.a)
        set(PROTOBUF_LIBRARIES
                ${CMAKE_SOURCE_DIR}/external/android_x86_64/protobuf/lib/libprotobuf.a)
    else ()
        message(FATAL_ERROR "Unsupported CPU Architecture - ${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    target_sources(keysaver-core
        PUBLIC
            keysaver/interface.h
            keysaver/interface_android.h
        PRIVATE
            interface_android.cpp
            ${IMPLEMENTATION}
            native-lib.cpp #TODO - remove it
    )

    target_include_directories(keysaver-core
        PRIVATE
            ${OPENSSL_INCLUDE_DIR}
    )

    target_link_libraries(keysaver-core
        PRIVATE
            android
            ${OPENSSL_LIBRARIES}
            ${PROTOBUF_LIBRARIES}
    )
endif()


