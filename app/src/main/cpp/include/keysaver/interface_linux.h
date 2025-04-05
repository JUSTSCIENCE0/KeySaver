// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#define KEYSAVER_API(func_name, ...) \
    extern "C" KeysaverStatus func_name(__VA_ARGS__)

#define keysaverString       const char*
#define keysaverStringRef    char*
#define keysaverService      KeysaverService
#define keysaverServiceRef   KeysaverService*
#define keysaverConfig       KeysaverConfiguration
#define keysaverInt          int
#define keysaverUInt         uint
#define keysaverBool         bool
#define keysaverIntRef       int*
#define keysaverServicesList char* const*
#define keysaverConfigsList  char* const*
#define keysaverAlphabetList char* const*

