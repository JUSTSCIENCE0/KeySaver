// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <jni.h>

#define KEYSAVER_API(func_name, ...) \
    extern "C" JNIEXPORT jint JNICALL Java_com_science_keysaver_Implementation_ ##func_name( \
        [[maybe_unused]] JNIEnv* j_env, [[maybe_unused]] jobject j_this \
        __VA_OPT__(,) __VA_ARGS__)

#define keysaverChar         jchar
#define keysaverString       jstring
#define keysaverStringRef    jobject
#define keysaverService      jobject
#define keysaverServiceRef   jobject
#define keysaverConfig       jobject
#define keysaverInt          jint
#define keysaverUInt         jint
#define keysaverBool         jboolean
#define keysaverIntRef       jobject
#define keysaverServicesList jobjectArray
#define keysaverConfigsList  jobjectArray
#define keysaverAlphabetList jobjectArray

