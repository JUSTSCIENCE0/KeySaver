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

enum KeysaverError {
    OK = 0,
    TOO_SHORT_MASTER_PASSWORD = -1,
    INVALID_MASTER_PASSWORD = -2
};

//KEYSAVER_API_EX(keysaverSetMasterKey, jshortArray unicodePassword);
