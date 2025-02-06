// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "engine.hpp"

static Keysaver::Engine* ks_impl = nullptr;

KeysaverStatus jobj_to_service(
        JNIEnv* j_env, jobject jservice, KeysaverConfig::Service* cservice) {
    auto serviceClass = j_env->GetObjectClass(jservice);
    auto serviceUrlField = j_env->GetFieldID(
            serviceClass,
            "url",
            "Ljava/lang/String;");
    auto serviceNameField = j_env->GetFieldID(
            serviceClass,
            "name",
            "Ljava/lang/String;");
    auto serviceConfigField = j_env->GetFieldID(
            serviceClass,
            "conf_id",
            "Ljava/lang/String;");

    auto j_url = static_cast<jstring>(
            j_env->GetObjectField(jservice, serviceUrlField));
    auto j_name = static_cast<jstring>(
            j_env->GetObjectField(jservice, serviceNameField));
    auto j_conf_id = static_cast<jstring>(
            j_env->GetObjectField(jservice, serviceConfigField));
    if (!j_url || !j_name || !j_conf_id)
        return KeysaverStatus::E_INVALID_ARG;

    auto c_url = j_env->GetStringUTFChars(j_url, nullptr);
    auto c_name = j_env->GetStringUTFChars(j_name, nullptr);
    auto c_conf_id = j_env->GetStringUTFChars(j_conf_id, nullptr);
    if (!c_url || !c_name || !c_conf_id)
        return KeysaverStatus::E_INVALID_ARG;

    cservice->set_url(c_url);
    cservice->set_name(c_name);
    cservice->set_conf_id(c_conf_id);
    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverInit, jstring configPath) {
    auto c_config_path = j_env->GetStringUTFChars(configPath, nullptr);
    try {
        ks_impl = &Keysaver::Engine::Get(c_config_path);
    }
    catch (KeysaverStatus code) {
        return code;
    }
    catch (...) {
        return KeysaverStatus::E_NOT_INITIALIZED;
    }

    return ks_impl->FirstUsage();
}

KEYSAVER_API(keysaverClose) {
    if (!ks_impl) return KeysaverStatus::S_OK;

    ks_impl->Invalidate();
    ks_impl = nullptr;
    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverSetMasterPassword, jstring masterPassword){
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    auto c_master_password =
            j_env->GetStringUTFChars(masterPassword, nullptr);
    return ks_impl->SetMasterPassword(c_master_password);
}

KEYSAVER_API(keysaverAddService, jobject service) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    KeysaverConfig::Service servConf;
    auto code = jobj_to_service(j_env, service, &servConf);
    if (is_keysaver_error(code)) return code;

    return ks_impl->AddService(servConf);
}

KEYSAVER_API(keysaverDeleteService, jstring serviceName) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    auto c_service_name =
            j_env->GetStringUTFChars(serviceName, nullptr);
    return ks_impl->DeleteService(c_service_name);
}

KEYSAVER_API(keysaverEditService,       jstring oldServiceName,
             jobject newService) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    auto c_service_name = j_env->GetStringUTFChars(oldServiceName, nullptr);
    KeysaverConfig::Service servConf;
    auto code = jobj_to_service(j_env, newService, &servConf);
    if (is_keysaver_error(code)) return code;

    return ks_impl->EditService(c_service_name, servConf);
}

KEYSAVER_API(keysaverSyncDatabase) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    return ks_impl->SyncDatabase();
}

KEYSAVER_API(keysaverGetServicesCount, jobject servicesCount) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    size_t serv_cnt = 0;
    auto code = ks_impl->GetServicesCount(&serv_cnt);
    if (is_keysaver_error(code)) return code;

    auto wrapperClass = j_env->GetObjectClass(servicesCount);
    auto valueField = j_env->GetFieldID(wrapperClass, "value", "I");
    j_env->SetIntField(
            servicesCount,
            valueField,
            static_cast<int>(serv_cnt));

    return code;
}

KEYSAVER_API(keysaverGetServicesList, jobjectArray servicesList) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    std::list<std::string> c_services_list;
    auto code = ks_impl->GetServicesList(&c_services_list);
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

KEYSAVER_API(keysaverGetService, jstring serviceName, jobject service) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    auto c_service_name = j_env->GetStringUTFChars(serviceName, nullptr);

    auto serviceClass = j_env->GetObjectClass(service);
    auto serviceNameField = j_env->GetFieldID(
            serviceClass,
            "name",
            "Ljava/lang/String;");
    auto serviceConfigField = j_env->GetFieldID(
            serviceClass,
            "conf_id",
            "Ljava/lang/String;");

    if (!serviceNameField || !serviceConfigField)
        return KeysaverStatus::E_INVALID_ARG;

    const KeysaverConfig::Service* c_service = nullptr;
    auto code = ks_impl->GetService(c_service_name, &c_service);
    if (is_keysaver_error(code)) return code;

    j_env->SetObjectField(
            service,
            serviceNameField,
            j_env->NewStringUTF(c_service->name().c_str()));
    j_env->SetObjectField(
            service,
            serviceConfigField,
            j_env->NewStringUTF(c_service->conf_id().c_str()));

    return code;
}

KEYSAVER_API(keysaverGetConfigurationsCount, jobject configurationsCount) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    size_t conf_cnt = 0;
    auto code = ks_impl->GetConfigurationsCount(&conf_cnt);
    if (is_keysaver_error(code)) return code;

    auto wrapperClass = j_env->GetObjectClass(configurationsCount);
    auto valueField = j_env->GetFieldID(wrapperClass, "value", "I");
    j_env->SetIntField(
        configurationsCount,
            valueField,
            static_cast<int>(conf_cnt));

    return code;
}

KEYSAVER_API(keysaverGetConfigurationsList, jobjectArray configurationsList) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    std::list<std::string> c_conf_list;
    auto code = ks_impl->GetConfigurationsList(&c_conf_list);
    if (is_keysaver_error(code)) return code;

    jsize index = 0;
    for (auto& config: c_conf_list) {
        jstring str = j_env->NewStringUTF(config.c_str());
        j_env->SetObjectArrayElement(configurationsList, index, str);
        j_env->DeleteLocalRef(str);

        index++;
    }

    return code;
}

KEYSAVER_API(keysaverGetAlphabetsCount, jobject alphabetsCount) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    size_t alphabets_cnt = Keysaver::Engine::SUPPORTED_ALPHABETS.size();

    auto wrapperClass = j_env->GetObjectClass(alphabetsCount);
    auto valueField = j_env->GetFieldID(wrapperClass, "value", "I");
    j_env->SetIntField(
            alphabetsCount,
            valueField,
    static_cast<int>(alphabets_cnt));

    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverGetAlphabetsList, jobjectArray alphabetsList) {
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    jsize index = 0;
    for (auto& alphabet: Keysaver::Engine::SUPPORTED_ALPHABETS) {
        jstring str = j_env->NewStringUTF(reinterpret_cast<const char*>(alphabet.data()));
        j_env->SetObjectArrayElement(alphabetsList, index, str);
        j_env->DeleteLocalRef(str);

        index++;
    }

    return KeysaverStatus::S_OK;
}
