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
    S_IS_FOUND = 0,

    // Messages
    M_DATABASE_NOT_FOUND = 1,

    // Errors
    E_INVALID_MASTER_PASSWORD    = -1,
    E_TOO_SHORT_MASTER_PASSWORD  = -2,
    E_SERVICE_ALREADY_EXISTS     = -3,
    E_SERVICE_URL_ALREADY_EXISTS = -4,
    E_CONFIG_ALREADY_EXISTS      = -5,

    // Internal Errors
    E_NOT_IMPLEMENTED       = -1000,
    E_NOT_INITIALIZED       = -1001,
    E_INTERNAL_OPENSSL_FAIL = -1002,
    E_INVALID_ARG           = -1003,
    E_SERVICE_NOT_EXISTS    = -1004,
    E_CONFIG_NOT_EXISTS     = -1005,
    E_DB_WRITE_ERROR        = -1006,
    E_DB_READ_ERROR         = -1007,
    E_DB_CORRUPTED          = -1008,
};

static inline bool is_keysaver_error(KeysaverStatus code) {
    return code < 0;
}

static inline bool is_keysaver_success(KeysaverStatus code) {
    return code == 0;
}

KEYSAVER_API(keysaverInit, jstring configPath);

KEYSAVER_API(keysaverSetMasterPassword, jstring masterPassword);
KEYSAVER_API(keysaverAddService,        jobject service);
KEYSAVER_API(keysaverSyncDatabase);

KEYSAVER_API(keysaverGetServicesCount,       jobject      servicesCount);
KEYSAVER_API(keysaverGetServicesList,        jobjectArray servicesList);
KEYSAVER_API(keysaverGetConfigurationsCount, jobject      configurationsCount);
KEYSAVER_API(keysaverGetConfigurationsList,  jobjectArray configurationsList);

