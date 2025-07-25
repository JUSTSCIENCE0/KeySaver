# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22.1)

project(keysaver-core)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
    include/keysaver/interface.h
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
    set(OPENSSL_CONFIGURE ./Configure)
    set(OPENSSL_BUILDER make)
    set(OPENSSL_LIBSSL_NAME libssl.a)
    set(OPENSSL_LIBCRYPTO_NAME libcrypto.a)
    set(PROTOBUF_USE_HOST_PROTOC TRUE)
    set(IMPLEMENTATION
        interface_android.cpp
        ${IMPLEMENTATION}
    )
    set(INTERFACE
        include/keysaver/interface_android.h
        ${INTERFACE}
    )
    set(SYSTEM_SPECIFIC_LIBS android)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" AND NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        message(FATAL_ERROR "Unsupported CPU Architecture - ${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    set(OPENSSL_CONFIGURE ./Configure)
    set(OPENSSL_BUILDER make)
    set(OPENSSL_LIBSSL_NAME libssl.a)
    set(OPENSSL_LIBCRYPTO_NAME libcrypto.a)
    set(IMPLEMENTATION
        interface_common.cpp
        ${IMPLEMENTATION}
    )
    set(INTERFACE
        include/keysaver/interface_linux.h
        ${INTERFACE}
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" AND NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        message(FATAL_ERROR "Unsupported CPU Architecture - ${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    set(OPENSSL_CONFIGURE perl Configure)
    set(OPENSSL_BUILDER nmake)
    set(OPENSSL_LIBSSL_NAME libssl.lib)
    set(OPENSSL_LIBCRYPTO_NAME libcrypto.lib)
    set(IMPLEMENTATION
        interface_common.cpp
        ${IMPLEMENTATION}
    )
    set(INTERFACE
        include/keysaver/interface_windows.h
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
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND
        ${OPENSSL_ENVIRONMENT}
        ${OPENSSL_CONFIGURE} ${OPENSSL_PLATFORM_PARAMS} no-apps no-docs no-shared no-tests
            --prefix=${OPENSSL_INSTALL_DIR} --openssldir=${OPENSSL_INSTALL_DIR}/ssl --libdir=lib
    BUILD_COMMAND ${OPENSSL_ENVIRONMENT} ${OPENSSL_BUILDER}
    INSTALL_COMMAND ${OPENSSL_ENVIRONMENT} ${OPENSSL_BUILDER} install_sw
    BUILD_BYPRODUCTS
        ${OPENSSL_INSTALL_DIR}/lib/${OPENSSL_LIBSSL_NAME}
        ${OPENSSL_INSTALL_DIR}/lib/${OPENSSL_LIBCRYPTO_NAME}
)
file(MAKE_DIRECTORY ${OPENSSL_INSTALL_DIR}/include)
add_library(OpenSSL::SSL STATIC IMPORTED)
add_dependencies(OpenSSL::SSL OpenSSL)
set_target_properties(OpenSSL::SSL PROPERTIES
        IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/${OPENSSL_LIBSSL_NAME}
        INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INSTALL_DIR}/include)
add_library(OpenSSL::Crypto STATIC IMPORTED)
add_dependencies(OpenSSL::Crypto OpenSSL)
set_target_properties(OpenSSL::Crypto PROPERTIES
        IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/${OPENSSL_LIBCRYPTO_NAME}
        INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INSTALL_DIR}/include)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

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
endif(PROTOBUF_USE_HOST_PROTOC)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(protobuf_MSVC_STATIC_RUNTIME OFF)
endif() # MSVC

FetchContent_MakeAvailable(protobuf)

# keysaver core
add_library(keysaver-core SHARED)

#  generate protobuf sources
if (PROTOBUF_USE_HOST_PROTOC)
    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        set(PROTOC_BUILDER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/build_host_protobuf.sh)
        set(PROTOC_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/external/linux_x86_64/protobuf/bin/protoc)
    else()
        message(FATAL_ERROR "Unsupported host OS")
    endif ()
    if (NOT CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64" AND
            NOT CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "AMD64")
        message(FATAL_ERROR "Unsupported host processor")
    endif()

    if(NOT EXISTS ${PROTOC_EXECUTABLE})
        execute_process(
            COMMAND bash ${PROTOC_BUILDER_SCRIPT}
            WORKING_DIRECTORY ${${CMAKE_CURRENT_SOURCE_DIR}}
            RESULT_VARIABLE SCRIPT_RESULT
        )

        if(NOT SCRIPT_RESULT EQUAL 0)
            message(FATAL_ERROR "Protobuf build failed with code ${SCRIPT_RESULT}")
        endif()
    endif()

    add_custom_command(
        OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/configuration.pb.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/configuration.pb.h
        COMMAND ${PROTOC_EXECUTABLE}
        ARGS --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}
            --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
        configuration.proto
    )
else(PROTOBUF_USE_HOST_PROTOC)
    include(${protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)
    protobuf_generate(
        TARGET keysaver-core
        LANGUAGE cpp
        IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        PROTOS configuration.proto
        PROTOC_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

# keysaver core compile options
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(keysaver-core PRIVATE
        # warnings options
        /W4 /WX
        # disable protobuf warnings
        /wd4100 /wd4267 /wd4127 /wd4189
        /DUNICODE /D_UNICODE /utf-8 # letters encoding
        /MP                     # multiple processes build
    )
    target_compile_definitions(keysaver-core PRIVATE
        _CRT_SECURE_NO_WARNINGS)
elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    target_compile_options(keysaver-core PRIVATE
        # warnings options
        -Wall -Wextra -Wcast-align -Wcast-qual
        -Wdisabled-optimization -Wformat=2 -Winit-self
        -Wmissing-include-dirs -Woverloaded-virtual
        -Wsign-promo -fdiagnostics-show-option
        -Werror
    )
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(keysaver-core PRIVATE -Wno-gcc-compat)
    else()
        target_compile_options(keysaver-core PRIVATE -Wno-missing-requires)
    endif()
endif() # GCC & Clang

# keysaver core sources
target_sources(keysaver-core
    PUBLIC
        ${INTERFACE}
    PRIVATE
        ${IMPLEMENTATION}
)

target_include_directories(keysaver-core
    PUBLIC
        include
)

# keysaver core dependencies
add_dependencies(keysaver-core OpenSSL)
target_link_libraries(keysaver-core
    PRIVATE
        ${SYSTEM_SPECIFIC_LIBS}
        protobuf::libprotobuf
        OpenSSL::SSL
        OpenSSL::Crypto
)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(keysaver-core
    PRIVATE
        Ws2_32.lib
        Crypt32.lib
)
endif() # MSVC
