// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "implementation.hpp"

static Keysaver::Implementation& ks_impl =
        Keysaver::Implementation::Get();

KEYSAVER_API(keysaverInit, jstring configPath) {
    auto c_config_path = j_env->GetStringUTFChars(configPath, nullptr);
    return ks_impl.Init(c_config_path);
}

KEYSAVER_API(keysaverSetMasterPassword, jstring masterPassword){
    auto c_master_password =
            j_env->GetStringUTFChars(masterPassword, nullptr);
    return ks_impl.SetMasterPassword(c_master_password);
}
