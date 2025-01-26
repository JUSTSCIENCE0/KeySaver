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
    size_t serv_cnt = 0;
    auto code = ks_impl.GetServicesCount(&serv_cnt);
    if (is_keysaver_error(code)) return code;

    jclass wrapperClass = j_env->GetObjectClass(servicesCount);
    jfieldID valueField = j_env->GetFieldID(wrapperClass, "value", "I");
    j_env->SetIntField(
            servicesCount,
            valueField,
            static_cast<int>(serv_cnt));

    return code;
}

KEYSAVER_API(keysaverGetServicesList, jobjectArray servicesList) {
    std::list<std::string> c_services_list;
    auto code = ks_impl.GetServicesList(&c_services_list);
    if (is_keysaver_error(code)) return code;

    jsize index = 0;
    for (auto& service: c_services_list) {
        jstring str = j_env->NewStringUTF(service.c_str());
        j_env->SetObjectArrayElement(servicesList, index, str);
        j_env->DeleteLocalRef(str);

        index++;
    }

    return code;
}
