// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <stdlib.h>

#if defined(__ANDROID__)
#include  "interface_android.h"
#elif defined(__linux__)
#include "interface_linux.h"
#elif defined(WIN32)
#include "interface_windows.h"
#endif

#ifndef KEYSAVER_API
#error "Unsupported OS"
#endif

#define KEYSAVER_STRING_MAX_SIZE 100

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
    E_TOO_LONG_STRING       = -1012,
};

struct KeysaverService {
    keysaverChar url[KEYSAVER_STRING_MAX_SIZE];
    keysaverChar name[KEYSAVER_STRING_MAX_SIZE];
    keysaverChar conf_id[KEYSAVER_STRING_MAX_SIZE];
};

struct KeysaverConfiguration {
    keysaverString id_name             = NULL;
    keysaverUInt   length              = 0;
    keysaverBool   use_upper           = false;
    keysaverBool   use_lower           = false;
    keysaverInt    alphabet            = 0;
    keysaverBool   use_special_chars   = false;
    keysaverUInt   special_chars_count = 0;
    keysaverString special_charset     = NULL;
    keysaverBool   use_digits          = false;
    keysaverUInt   digits_amount       = 0;
};

static inline bool is_keysaver_error(KeysaverStatus code) {
    return code < 0;
}

static inline bool is_keysaver_success(KeysaverStatus code) {
    return code == 0;
}

KEYSAVER_API(keysaverInit, keysaverString configPath);
KEYSAVER_API(keysaverClose);

KEYSAVER_API(keysaverSetMasterPassword, keysaverString  masterPassword);
KEYSAVER_API(keysaverAddService,        keysaverService service);
KEYSAVER_API(keysaverDeleteService,     keysaverString  serviceName);
KEYSAVER_API(keysaverEditService,       keysaverString  oldServiceName,
                                        keysaverService newService);
KEYSAVER_API(keysaverAddConfiguration,  keysaverConfig  confDescr);
KEYSAVER_API(keysaverSyncDatabase);

KEYSAVER_API(keysaverGetServicesCount,       keysaverIntRef       servicesCount);
KEYSAVER_API(keysaverGetServicesList,        keysaverServicesList servicesList);
KEYSAVER_API(keysaverGetService,             keysaverString       serviceName,
                                             keysaverServiceRef   service);
KEYSAVER_API(keysaverGetConfigurationsCount, keysaverIntRef       configurationsCount);
KEYSAVER_API(keysaverGetConfigurationsList,  keysaverConfigsList  configurationsList);
KEYSAVER_API(keysaverGetAlphabetsCount,      keysaverIntRef       alphabetsCount);
KEYSAVER_API(keysaverGetAlphabetsList,       keysaverAlphabetList alphabetsList);
KEYSAVER_API(keysaverGetDatabaseName,        keysaverStringRef    fileName);

KEYSAVER_API(keysaverGeneratePassword, keysaverString    serviceName,
                                       keysaverInt       imageIndex,
                                       keysaverStringRef result);
