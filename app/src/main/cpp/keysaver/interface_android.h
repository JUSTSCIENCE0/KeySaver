// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <jni.h>

#define KEYSAVER_API(func_name, ...) \
    extern "C" JNIEXPORT jint JNICALL Java_com_science_keysaver_Implementation_ ##func_name( \
        JNIEnv* j_env, jobject j_this \
        __VA_OPT__(,) __VA_ARGS__)
