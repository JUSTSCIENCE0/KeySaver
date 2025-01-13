// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "implementation.hpp"

KEYSAVER_API(keysaverInit, jstring configPath) {
    auto c_config_path = j_env->GetStringUTFChars(configPath, nullptr);
    return Keysaver::init(c_config_path);
}

KEYSAVER_API(keysaverSetMasterPassword, jstring masterPassword){
    auto c_master_password =
            j_env->GetStringUTFChars(masterPassword, nullptr);
    return Keysaver::set_master_password(c_master_password);
}
