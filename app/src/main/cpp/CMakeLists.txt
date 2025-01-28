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

set(PROTOC_EXECUTABLE ${CMAKE_SOURCE_DIR}/external/linux_x86_64/protobuf/bin/protoc)
add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/configuration.pb.cc
               ${CMAKE_SOURCE_DIR}/configuration.pb.h
        COMMAND ${PROTOC_EXECUTABLE}
        ARGS --cpp_out=${CMAKE_SOURCE_DIR}
             --proto_path=${CMAKE_SOURCE_DIR}
             configuration.proto
)

set(IMPLEMENTATION
    engine.hpp
    engine.cpp
    db_manager.hpp
    db_manager.cpp

    # protobuf generated
    configuration.pb.cc
    configuration.pb.h
)

set(INTERFACE
    keysaver/interface.h
)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(PLATFORM android_arm64-v8a)
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        set(PLATFORM android_x86_64)
    else ()
        message(FATAL_ERROR "Unsupported CPU Architecture - ${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    set(IMPLEMENTATION
        interface_android.cpp
        ${IMPLEMENTATION}
    )

    set(INTERFACE
        keysaver/interface_android.h
        ${INTERFACE}
    )
else()
    message(FATAL_ERROR "Unsupported platform - ${CMAKE_SYSTEM_NAME}")
endif()

#externals
set(EXTERNALS_DIR ${CMAKE_SOURCE_DIR}/external/${PLATFORM})

# openssl
set(OpenSSL_DIR ${EXTERNALS_DIR}/openssl/lib/cmake/OpenSSL)
find_package(OpenSSL REQUIRED NO_MODULE)

# protobuf
set(protobuf_DIR   ${EXTERNALS_DIR}/protobuf/lib/cmake/protobuf)
set(absl_DIR       ${EXTERNALS_DIR}/protobuf/lib/cmake/absl)
set(utf8_range_DIR ${EXTERNALS_DIR}/protobuf/lib/cmake/utf8_range)
find_package(protobuf REQUIRED NO_MODULE)

add_library(keysaver-core SHARED)

target_sources(keysaver-core
    PUBLIC
        ${INTERFACE}
    PRIVATE
        ${IMPLEMENTATION}
)

target_link_libraries(keysaver-core
    PRIVATE
        android
        protobuf::libprotobuf
        OpenSSL::SSL
        OpenSSL::Crypto
)
