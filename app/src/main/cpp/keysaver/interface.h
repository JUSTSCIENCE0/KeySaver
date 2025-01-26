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
    E_INVALID_MASTER_PASSWORD = -1,
    E_TOO_SHORT_MASTER_PASSWORD = -2,

    // Internal Errors
    E_NOT_IMPLEMENTED       = -1000,
    E_NOT_INITIALIZED       = -1001,
    E_INTERNAL_OPENSSL_FAIL = -1002,
    E_INVALID_ARG           = -1003,
};

static inline bool is_keysaver_error(KeysaverStatus code) {
    return code < 0;
}

static inline bool is_keysaver_success(KeysaverStatus code) {
    return code == 0;
}

KEYSAVER_API(keysaverInit, jstring configPath);
KEYSAVER_API(keysaverSetMasterPassword, jstring masterPassword);

KEYSAVER_API(keysaverGetServicesCount, jobject servicesCount);
KEYSAVER_API(keysaverGetServicesList,  jobjectArray servicesList);
