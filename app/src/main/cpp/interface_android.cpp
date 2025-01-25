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

KEYSAVER_API(keysaverGetServicesCount, jobject servicesCount) {
    jclass wrapperClass = j_env->GetObjectClass(servicesCount);
    jfieldID valueField = j_env->GetFieldID(wrapperClass, "value", "I");
    j_env->SetIntField(servicesCount, valueField, 0);

    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverGetServicesList, jobjectArray servicesList) {
    std::vector<std::string> c_services_list = {"Hello", "World", "From", "C++"};

    jsize index = 0;
    for (auto& service: c_services_list) {
        jstring str = j_env->NewStringUTF(service.c_str());
        j_env->SetObjectArrayElement(servicesList, index, str);
        j_env->DeleteLocalRef(str);

        index++;
    }

    return KeysaverStatus::S_OK;
}
