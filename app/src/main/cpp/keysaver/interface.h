// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#if defined(__ANDROID__)
#include  "interface_android.h"
#endif

#ifndef KEYSAVER_API
#error "Unsupported OS"
#endif

enum KeysaverStatus {
    // SUCCESS
    S_OK = 0,

    // Messages
    M_CONFIG_NOT_FOUND = 1,

    // Errors
    E_NOT_IMPLEMENTED = -1,
    E_NOT_INITIALIZED = -2,
    E_TOO_SHORT_MASTER_PASSWORD = -3,
    E_INVALID_MASTER_PASSWORD = -4,
};

static inline bool is_keysaver_error(KeysaverStatus code) {
    return code < 0;
}

static inline bool is_keysaver_success(KeysaverStatus code) {
    return code == 0;
}

KEYSAVER_API(keysaverInit, jstring configPath);
KEYSAVER_API(keysaverSetMasterPassword, jstring masterPassword);
