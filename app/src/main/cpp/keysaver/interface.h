// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#if defined(__ANDROID__)
#include  "interface_android.h"
#elif defined(__linux__)
#include "interface_linux.h"
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
    E_INVALID_PASSWORD_LENGTH    = -6,
    E_INVALID_SPECIAL_CHAR_COUNT = -7,
    E_INVALID_DIGITS_AMOUNT      = -8,
    E_WITHOUT_ANY_CASE           = -9,

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
    E_UNSUPPORTED_ALPHABET  = -1009,
    E_UNEXPECTED_EXCEPTION  = -1010,
    E_INVALID_ORDER         = -1011,
};

static inline bool is_keysaver_error(KeysaverStatus code) {
    return code < 0;
}

static inline bool is_keysaver_success(KeysaverStatus code) {
    return code == 0;
}

KEYSAVER_API(keysaverInit, jstring configPath);
KEYSAVER_API(keysaverClose);

KEYSAVER_API(keysaverSetMasterPassword, jstring masterPassword);
KEYSAVER_API(keysaverAddService,        jobject service);
KEYSAVER_API(keysaverDeleteService,     jstring serviceName);
KEYSAVER_API(keysaverEditService,       jstring oldServiceName,
                                        jobject newService);
KEYSAVER_API(keysaverAddConfiguration,  jobject confDescr);
KEYSAVER_API(keysaverSyncDatabase);

KEYSAVER_API(keysaverGetServicesCount,       jobject      servicesCount);
KEYSAVER_API(keysaverGetServicesList,        jobjectArray servicesList);
KEYSAVER_API(keysaverGetService,             jstring      serviceName,
                                             jobject      service);
KEYSAVER_API(keysaverGetConfigurationsCount, jobject      configurationsCount);
KEYSAVER_API(keysaverGetConfigurationsList,  jobjectArray configurationsList);
KEYSAVER_API(keysaverGetAlphabetsCount,      jobject      alphabetsCount);
KEYSAVER_API(keysaverGetAlphabetsList,       jobjectArray alphabetsList);
KEYSAVER_API(keysaverGetDatabaseName,        jobject      fileName);

KEYSAVER_API(keysaverGeneratePassword,       jstring serviceName,
                                             jint imageIndex,
                                             jobject result);
