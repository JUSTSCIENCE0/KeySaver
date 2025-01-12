// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "keysaver/interface.h"

constexpr int MIN_PASSWORD_LENGTH = 8;

KEYSAVER_API(keysaverSetMasterKey, jshortArray unicodePassword){
    const jsize length = j_env->GetArrayLength(unicodePassword);
    if (length < MIN_PASSWORD_LENGTH)
        return KeysaverError::TOO_SHORT_MASTER_PASSWORD;

    return KeysaverError::OK;
}
