// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "engine.hpp"

static Keysaver::Engine* ks_impl = nullptr;

KeysaverStatus jobj_to_service(
        JNIEnv* j_env, jobject jservice, KeysaverConfig::Service* cservice) {
    if (!cservice)
        return KeysaverStatus::E_INVALID_ARG;

    auto serviceClass = j_env->GetObjectClass(jservice);
    if (!serviceClass)
        return KeysaverStatus::E_INVALID_ARG;

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

    if (!serviceUrlField ||
        !serviceNameField ||
        !serviceConfigField)
        return KeysaverStatus::E_INVALID_ARG;

    auto j_url = static_cast<jstring>(
            j_env->GetObjectField(jservice, serviceUrlField));
    auto j_name = static_cast<jstring>(
            j_env->GetObjectField(jservice, serviceNameField));
    auto j_conf_id = static_cast<jstring>(
            j_env->GetObjectField(jservice, serviceConfigField));
    if (!j_url || !j_name || !j_conf_id)
        return KeysaverStatus::E_INVALID_ARG;

    auto c_url = j_env->GetStringUTFChars(j_url, nullptr);
    if (!c_url)
        return KeysaverStatus::E_INVALID_ARG;
    auto c_name = j_env->GetStringUTFChars(j_name, nullptr);
    if (!c_name) {
        j_env->ReleaseStringUTFChars(j_url, c_url);
        return KeysaverStatus::E_INVALID_ARG;
    }
    auto c_conf_id = j_env->GetStringUTFChars(j_conf_id, nullptr);
    if (!c_conf_id) {
        j_env->ReleaseStringUTFChars(j_url, c_url);
        j_env->ReleaseStringUTFChars(j_name, c_name);
        return KeysaverStatus::E_INVALID_ARG;
    }

    cservice->set_url(c_url);
    cservice->set_name(c_name);
    cservice->set_conf_id(c_conf_id);

    j_env->ReleaseStringUTFChars(j_url, c_url);
    j_env->ReleaseStringUTFChars(j_name, c_name);
    j_env->ReleaseStringUTFChars(j_conf_id, c_conf_id);
    return KeysaverStatus::S_OK;
}

KeysaverStatus jobj_to_config(
        JNIEnv* j_env, jobject jconfig, KeysaverConfig::Configuration* cconfig) {
    if (!cconfig)
        return KeysaverStatus::E_INVALID_ARG;

    auto configClass = j_env->GetObjectClass(jconfig);
    if (!configClass)
        return KeysaverStatus::E_INVALID_ARG;

    auto conf_idField = j_env->GetFieldID(
            configClass, "conf_id", "Ljava/lang/String;");
    auto lengthField = j_env->GetFieldID(
            configClass, "length", "I");
    auto use_upperField = j_env->GetFieldID(
            configClass, "use_upper", "Z");
    auto use_lowerField = j_env->GetFieldID(
            configClass, "use_lower", "Z");
    auto alphabetField = j_env->GetFieldID(
            configClass, "alphabet", "Ljava/lang/String;");
    auto use_special_charsField = j_env->GetFieldID(
            configClass, "use_special_chars", "Z");
    auto special_chars_countField = j_env->GetFieldID(
            configClass, "special_chars_count", "I");
    auto special_charsetField = j_env->GetFieldID(
            configClass, "special_charset", "Ljava/lang/String;");
    auto use_digitsField = j_env->GetFieldID(
            configClass, "use_digits", "Z");
    auto digits_amountField = j_env->GetFieldID(
            configClass, "digits_amount", "I");

    if (!conf_idField || !lengthField || !use_upperField || !use_lowerField ||
        !alphabetField || !use_special_charsField || !special_chars_countField ||
        !special_charsetField || !use_digitsField || !digits_amountField)
        return KeysaverStatus::E_INVALID_ARG;

    auto j_conf_id = static_cast<jstring>(
            j_env->GetObjectField(jconfig, conf_idField));
    auto j_length = j_env->GetIntField(jconfig, lengthField);
    auto j_use_upper = j_env->GetBooleanField(jconfig, use_upperField);
    auto j_use_lower = j_env->GetBooleanField(jconfig, use_lowerField);
    auto j_alphabet = static_cast<jstring>(
            j_env->GetObjectField(jconfig, alphabetField));
    auto j_use_special_chars =
            j_env->GetBooleanField(jconfig, use_special_charsField);
    auto j_special_chars_count =
            j_env->GetIntField(jconfig, special_chars_countField);
    auto j_special_charset = static_cast<jstring>(
            j_env->GetObjectField(jconfig, special_charsetField));
    auto j_use_digits =
            j_env->GetBooleanField(jconfig, use_digitsField);
    auto j_digits_amount =
            j_env->GetIntField(jconfig, digits_amountField);

    auto c_conf_id = j_env->GetStringUTFChars(j_conf_id, nullptr);
    if (!c_conf_id)
        return KeysaverStatus::E_INVALID_ARG;
    auto c_alphabet = j_env->GetStringUTFChars(j_alphabet, nullptr);
    if (!c_alphabet)  {
        j_env->ReleaseStringUTFChars(j_conf_id, c_conf_id);
        return KeysaverStatus::E_INVALID_ARG;
    }
    auto c_special_charset=
            j_env->GetStringUTFChars(j_special_charset, nullptr);
    if (!c_special_charset) {
        j_env->ReleaseStringUTFChars(j_conf_id, c_conf_id);
        j_env->ReleaseStringUTFChars(j_alphabet, c_alphabet);
        return KeysaverStatus::E_INVALID_ARG;
    }

    std::string tmpAlphabetName(c_alphabet);
    std::u8string alphabetName{tmpAlphabetName.begin(), tmpAlphabetName.end()};
    auto alph_itr = std::find(
                              Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS.begin(),
                              Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS.end(),
                              alphabetName);
    if (Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS.end() == alph_itr)
        return KeysaverStatus::E_UNSUPPORTED_ALPHABET;
    auto alphabetIndex = std::distance(
            Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS.begin(), alph_itr);

    cconfig->set_id_name(c_conf_id);
    cconfig->set_length(j_length);
    cconfig->set_use_upper(j_use_upper);
    cconfig->set_use_lower(j_use_lower);
    cconfig->set_alphabet(KeysaverConfig::Configuration_AlphabetType(alphabetIndex));
    cconfig->set_use_special_chars(j_use_special_chars);
    cconfig->set_special_chars_count(j_special_chars_count);
    cconfig->set_special_charset(c_special_charset);
    cconfig->set_use_digits(j_use_digits);
    cconfig->set_digits_amount(j_digits_amount);

    j_env->ReleaseStringUTFChars(j_conf_id, c_conf_id);
    j_env->ReleaseStringUTFChars(j_alphabet, c_alphabet);
    j_env->ReleaseStringUTFChars(j_special_charset, c_special_charset);

    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverInit, keysaverString configPath) {
    auto c_config_path = j_env->GetStringUTFChars(configPath, nullptr);
    if (!c_config_path)
        return KeysaverStatus::E_INVALID_ARG;

    try {
        ks_impl = &Keysaver::Engine::Get(c_config_path);
    }
    catch (KeysaverStatus code) {
        j_env->ReleaseStringUTFChars(configPath, c_config_path);
        return code;
    }
    catch (...) {
        j_env->ReleaseStringUTFChars(configPath, c_config_path);
        return KeysaverStatus::E_UNEXPECTED_EXCEPTION;
    }

    j_env->ReleaseStringUTFChars(configPath, c_config_path);
    return ks_impl->FirstUsage();
}

KEYSAVER_API(keysaverClose) {
    if (!ks_impl) return KeysaverStatus::S_OK;

    ks_impl->Invalidate();
    ks_impl = nullptr;
    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverSetMasterPassword, keysaverString masterPassword){
    if (!ks_impl) return KeysaverStatus::E_NOT_INITIALIZED;

    auto c_master_password =
            j_env->GetStringUTFChars(masterPassword, nullptr);
    if (!c_master_password)
        return KeysaverStatus::E_INVALID_ARG;

    auto code = ks_impl->SetMasterPassword(c_master_password);

    j_env->ReleaseStringUTFChars(masterPassword, c_master_password);
    return code;
}

KEYSAVER_API(keysaverAddService, keysaverService service) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    KeysaverConfig::Service servConf;
    auto code = jobj_to_service(j_env, service, &servConf);
    if (is_keysaver_error(code))
        return code;

    return ks_impl->AddService(servConf);
}

KEYSAVER_API(keysaverDeleteService, keysaverString serviceName) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    auto c_service_name =
            j_env->GetStringUTFChars(serviceName, nullptr);
    if (!c_service_name)
        return KeysaverStatus::E_INVALID_ARG;

    auto code = ks_impl->DeleteService(c_service_name);

    j_env->ReleaseStringUTFChars(serviceName, c_service_name);
    return code;
}

KEYSAVER_API(keysaverEditService, keysaverString oldServiceName, keysaverService newService) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    KeysaverConfig::Service servConf;
    auto code = jobj_to_service(j_env, newService, &servConf);
    if (is_keysaver_error(code))
        return code;

    auto c_service_name = j_env->GetStringUTFChars(
        oldServiceName, nullptr);
    if (!c_service_name)
        return KeysaverStatus::E_INVALID_ARG;

    code = ks_impl->EditService(c_service_name, servConf);

    j_env->ReleaseStringUTFChars(oldServiceName, c_service_name);
    return code;
}

KEYSAVER_API(keysaverAddConfiguration, keysaverConfig confDescr) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    KeysaverConfig::Configuration config;
    auto code = jobj_to_config(j_env, confDescr, &config);
    if (is_keysaver_error(code))
        return code;

    return ks_impl->AddConfiguration(config);
}

KEYSAVER_API(keysaverSyncDatabase) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    return ks_impl->SyncDatabase();
}

KEYSAVER_API(keysaverGetServicesCount, keysaverIntRef servicesCount) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    Keysaver::DBManager::ServicesNames services;
    auto code = ks_impl->GetServicesList(&services);
    if (is_keysaver_error(code))
        return code;

    auto wrapperClass = j_env->GetObjectClass(servicesCount);
    auto valueField = j_env->GetFieldID(wrapperClass, "value", "I");
    j_env->SetIntField(
            servicesCount,
            valueField,
            static_cast<int>(services.size()));

    return code;
}

KEYSAVER_API(keysaverGetServicesList, keysaverServicesList servicesList) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    std::list<std::string> c_services_list;
    auto code = ks_impl->GetServicesList(&c_services_list);
    if (is_keysaver_error(code))
        return code;

    const auto max_index = j_env->GetArrayLength(servicesList);

    jsize index = 0;
    for (auto& service: c_services_list) {
        jstring str = j_env->NewStringUTF(service.c_str());
        j_env->SetObjectArrayElement(servicesList, index, str);
        j_env->DeleteLocalRef(str);

        index++;
        if (index >= max_index)
            break;
    }

    return code;
}

KEYSAVER_API(keysaverGetService, keysaverString serviceName, keysaverServiceRef service) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    auto c_service_name = j_env->GetStringUTFChars(serviceName, nullptr);
    if (!c_service_name)
        return KeysaverStatus::E_INVALID_ARG;

    auto serviceClass = j_env->GetObjectClass(service);
    if (!serviceClass) {
        j_env->ReleaseStringUTFChars(serviceName, c_service_name);
        return KeysaverStatus::E_INVALID_ARG;
    }

    auto serviceNameField = j_env->GetFieldID(
            serviceClass,
            "name",
            "Ljava/lang/String;");
    auto serviceConfigField = j_env->GetFieldID(
            serviceClass,
            "conf_id",
            "Ljava/lang/String;");

    if (!serviceNameField || !serviceConfigField) {
        j_env->ReleaseStringUTFChars(serviceName, c_service_name);
        return KeysaverStatus::E_INVALID_ARG;
    }

    KeysaverConfig::Service c_service;
    auto code = ks_impl->GetService(c_service_name, &c_service);
    if (is_keysaver_error(code)) {
        j_env->ReleaseStringUTFChars(serviceName, c_service_name);
        return code;
    }

    auto j_service_name = j_env->NewStringUTF(c_service.name().c_str());
    j_env->SetObjectField(
            service,
            serviceNameField,
        j_service_name);
    j_env->DeleteLocalRef(j_service_name);

    auto j_service_conf_id = j_env->NewStringUTF(c_service.conf_id().c_str());
    j_env->SetObjectField(
            service,
            serviceConfigField,
        j_service_conf_id);
    j_env->DeleteLocalRef(j_service_conf_id);

    j_env->ReleaseStringUTFChars(serviceName, c_service_name);
    return code;
}

KEYSAVER_API(keysaverGetConfigurationsCount, keysaverIntRef configurationsCount) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    Keysaver::DBManager::ConfigurationsNames configs;
    auto code = ks_impl->GetConfigurationsList(&configs);
    if (is_keysaver_error(code))
        return code;

    auto wrapperClass = j_env->GetObjectClass(configurationsCount);
    auto valueField = j_env->GetFieldID(wrapperClass, "value", "I");
    j_env->SetIntField(
        configurationsCount,
            valueField,
            static_cast<int>(configs.size()));

    return code;
}

KEYSAVER_API(keysaverGetConfigurationsList, keysaverConfigsList configurationsList) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    std::list<std::string> c_conf_list;
    auto code = ks_impl->GetConfigurationsList(&c_conf_list);
    if (is_keysaver_error(code))
        return code;

    const auto max_index = j_env->GetArrayLength(configurationsList);

    jsize index = 0;
    for (auto& config: c_conf_list) {
        jstring str = j_env->NewStringUTF(config.c_str());
        j_env->SetObjectArrayElement(configurationsList, index, str);
        j_env->DeleteLocalRef(str);

        index++;
        if (index >= max_index)
            break;
    }

    return code;
}

KEYSAVER_API(keysaverGetAlphabetsCount, keysaverIntRef alphabetsCount) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    size_t alphabets_cnt = Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS.size();

    auto wrapperClass = j_env->GetObjectClass(alphabetsCount);
    auto valueField = j_env->GetFieldID(wrapperClass, "value", "I");
    j_env->SetIntField(
            alphabetsCount,
            valueField,
    static_cast<int>(alphabets_cnt));

    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverGetAlphabetsList, keysaverAlphabetList alphabetsList) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    jsize index = 0;
    for (auto& alphabet: Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS) {
        std::string tmpAlphabetName{alphabet.begin(), alphabet.end()};
        jstring str = j_env->NewStringUTF(tmpAlphabetName.c_str());
        j_env->SetObjectArrayElement(alphabetsList, index, str);
        j_env->DeleteLocalRef(str);

        index++;
    }

    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverGetDatabaseName, keysaverStringRef fileName) {
    jclass resultClass = j_env->GetObjectClass(fileName);
    if (!resultClass)
        return KeysaverStatus::E_INVALID_ARG;
    jfieldID resultField = j_env->GetFieldID(
            resultClass, "value", "Ljava/lang/String;");
    if (!resultField)
        return KeysaverStatus::E_INVALID_ARG;

    std::string c_file_name = Keysaver::DBManager::DB_NAME;
    auto j_file_name = j_env->NewStringUTF(c_file_name.data());
    j_env->SetObjectField(fileName, resultField, j_file_name);
    j_env->DeleteLocalRef(j_file_name);

    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverGeneratePassword,
             keysaverString serviceName, keysaverInt imageIndex, keysaverStringRef result) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    auto c_service_name = j_env->GetStringUTFChars(serviceName, nullptr);
    if (!c_service_name)
        return KeysaverStatus::E_INVALID_ARG;

    jclass resultClass = j_env->GetObjectClass(result);
    if (!resultClass) {
        j_env->ReleaseStringUTFChars(serviceName, c_service_name);
        return KeysaverStatus::E_INVALID_ARG;
    }

    jfieldID resultField = j_env->GetFieldID(
            resultClass, "value", "Ljava/lang/String;");
    if (!resultField) {
        j_env->ReleaseStringUTFChars(serviceName, c_service_name);
        return KeysaverStatus::E_INVALID_ARG;
    }

    std::u8string cpp_result;
    auto code = ks_impl->GeneratePassword(
            c_service_name, imageIndex, &cpp_result);
    if (is_keysaver_error(code)) {
        j_env->ReleaseStringUTFChars(serviceName, c_service_name);
        return code;
    }

    std::string c_result{cpp_result.begin(), cpp_result.end()};
    auto j_result = j_env->NewStringUTF(c_result.c_str());
    j_env->SetObjectField(result, resultField, j_result);
    j_env->DeleteLocalRef(j_result);

    j_env->ReleaseStringUTFChars(serviceName, c_service_name);
    return code;
}
