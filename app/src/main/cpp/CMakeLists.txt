# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22.1)

project(keysaver-core)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(IMPLEMENTATION
    engine.hpp
    engine.cpp
    db_manager.hpp
    db_manager.cpp
    crypto_provider.hpp
    crypto_provider.cpp
    password_generator.hpp
    password_generator.cpp

    # protobuf generated
    configuration.pb.cc
    configuration.pb.h
)

set(INTERFACE
    keysaver/interface.h
)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(OPENSSL_PLATFORM_PARAMS android-arm64)
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        set(OPENSSL_PLATFORM_PARAMS android-x86_64)
    else ()
        message(FATAL_ERROR "Unsupported CPU Architecture - ${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    set(OPENSSL_PLATFORM_PARAMS
            ${OPENSSL_PLATFORM_PARAMS} -D__ANDROID_API__=${ANDROID_PLATFORM_LEVEL})
    set(OPENSSL_ENVIRONMENT
            ANDROID_NDK_ROOT=${ANDROID_NDK}
            PATH=${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin:$ENV{PATH})
    set(PROTOBUF_USE_HOST_PROTOC TRUE)
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
include(FetchContent)
include(ExternalProject)
set(OPENSSL_PACKAGE openssl-3.4.0)
set(OPENSSL_INSTALL_DIR ${CMAKE_BINARY_DIR}/openssl)

# openssl
ExternalProject_Add(
    OpenSSL
    PREFIX ${OPENSSL_INSTALL_DIR}
    URL https://github.com/openssl/openssl/releases/download/${OPENSSL_PACKAGE}/${OPENSSL_PACKAGE}.tar.gz
    BUILD_IN_SOURCE true
    CONFIGURE_COMMAND
        ${OPENSSL_ENVIRONMENT}
        ./Configure ${OPENSSL_PLATFORM_PARAMS} no-apps no-docs no-shared no-tests
            --prefix=${OPENSSL_INSTALL_DIR} --openssldir=${OPENSSL_INSTALL_DIR}/ssl
    BUILD_COMMAND ${OPENSSL_ENVIRONMENT} make -j${CMAKE_BUILD_PARALLEL_LEVEL}
    INSTALL_COMMAND ${OPENSSL_ENVIRONMENT} make install_sw
    BUILD_BYPRODUCTS
        ${OPENSSL_INSTALL_DIR}/lib/libssl.a
        ${OPENSSL_INSTALL_DIR}/lib/libcrypto.a
)
file(MAKE_DIRECTORY ${OPENSSL_INSTALL_DIR}/include)
add_library(OpenSSL::SSL STATIC IMPORTED)
add_dependencies(OpenSSL::SSL OpenSSL)
set_target_properties(OpenSSL::SSL PROPERTIES
        IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libssl.a
        INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INSTALL_DIR}/include)
add_library(OpenSSL::Crypto STATIC IMPORTED)
add_dependencies(OpenSSL::Crypto OpenSSL)
set_target_properties(OpenSSL::Crypto PROPERTIES
        IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libcrypto.a
        INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INSTALL_DIR}/include)

# protobuf
FetchContent_Declare(
    protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG v29.3
)
set(protobuf_BUILD_TESTS OFF)
set(protobuf_BUILD_CONFORMANCE OFF)
set(protobuf_BUILD_EXAMPLES OFF)
set(protobuf_DISABLE_RTTI ON)
if (PROTOBUF_USE_HOST_PROTOC)
    set(protobuf_BUILD_PROTOC_BINARIES OFF)
endif()
FetchContent_MakeAvailable(protobuf)

if (PROTOBUF_USE_HOST_PROTOC)
    if (NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        message(FATAL_ERROR "Unsupported host OS")
    endif ()
    if (NOT CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64" AND
            NOT CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "AMD64")
        message(FATAL_ERROR "Unsupported host processor")
    endif()

    set(PROTOC_EXECUTABLE ${CMAKE_SOURCE_DIR}/external/linux_x86_64/protobuf/bin/protoc)
    if(NOT EXISTS ${PROTOC_EXECUTABLE})
        execute_process(
            COMMAND bash build_host_protobuf.sh
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE SCRIPT_RESULT
        )

        if(NOT SCRIPT_RESULT EQUAL 0)
            message(FATAL_ERROR "Protobuf build failed with code ${SCRIPT_RESULT}")
        endif()
    endif()

    add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/configuration.pb.cc
        ${CMAKE_SOURCE_DIR}/configuration.pb.h
        COMMAND ${PROTOC_EXECUTABLE}
        ARGS --cpp_out=${CMAKE_SOURCE_DIR}
        --proto_path=${CMAKE_SOURCE_DIR}
        configuration.proto
    )
else()
    #TODO
endif()

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
        -Wstrict-overflow=5
        -fdiagnostics-show-option
        -Werror
    )
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wno-gcc-compat)
    endif()
endif() # GCC & Clang

add_library(keysaver-core SHARED)

target_sources(keysaver-core
    PUBLIC
        ${INTERFACE}
    PRIVATE
        ${IMPLEMENTATION}
)

add_dependencies(keysaver-core OpenSSL)
target_include_directories(keysaver-core
    PRIVATE
        ${protobuf_SOURCE_DIR}/src
)
target_link_libraries(keysaver-core
    PRIVATE
        android
        protobuf::libprotobuf
        OpenSSL::SSL
        OpenSSL::Crypto
)
