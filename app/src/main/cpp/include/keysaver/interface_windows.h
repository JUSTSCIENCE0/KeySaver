// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#ifdef keysaver_core_EXPORTS
# define EXPORT_TAG __declspec(dllexport)
#else
# define EXPORT_TAG __declspec(dllimport)
#endif // keysaver_core_EXPORTS


#define KEYSAVER_API(func_name, ...) \
    extern "C" EXPORT_TAG KeysaverStatus func_name(__VA_ARGS__)

#define keysaverChar         char
#define keysaverString       const char*
#define keysaverStringRef    char*
#define keysaverService      KeysaverService
#define keysaverServiceRef   KeysaverService*
#define keysaverConfig       KeysaverConfiguration
#define keysaverInt          int
#define keysaverUInt         unsigned int
#define keysaverBool         bool
#define keysaverIntRef       int*
#define keysaverServicesList char* const*
#define keysaverConfigsList  char* const*
#define keysaverAlphabetList char* const*

