// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "engine.hpp"

static Keysaver::Engine* ks_impl = nullptr;

KeysaverConfig::Service ConvertService(const KeysaverService& service) {
    KeysaverConfig::Service result;
    result.set_url(service.url);
    result.set_name(service.name);
    result.set_conf_id(service.conf_id);

    return result;
}

KeysaverService ConvertService(const KeysaverConfig::Service& service) {
    KeysaverService result{};

    assert(service.url().length() < KEYSAVER_STRING_MAX_SIZE);
    assert(service.name().length() < KEYSAVER_STRING_MAX_SIZE);
    assert(service.conf_id().length() < KEYSAVER_STRING_MAX_SIZE);

    std::strcpy(result.url, service.url().c_str());
    std::strcpy(result.name, service.name().c_str());
    std::strcpy(result.conf_id, service.conf_id().c_str());

    return result;
}

KeysaverConfig::Configuration ConvertConfiguration(
    const KeysaverConfiguration& config) {
    KeysaverConfig::Configuration result;
    result.set_id_name(config.id_name);
    result.set_length(config.length);
    result.set_use_upper(config.use_upper);
    result.set_use_lower(config.use_lower);
    result.set_alphabet(KeysaverConfig::Configuration_AlphabetType(config.alphabet));
    result.set_use_special_chars(config.use_special_chars);
    result.set_special_chars_count(config.special_chars_count);
    result.set_special_charset(config.special_charset);
    result.set_use_digits(config.use_digits);
    result.set_digits_amount(config.digits_amount);

    return result;
}

KeysaverConfiguration ConvertConfiguration(
    const KeysaverConfig::Configuration& config) {
    return {
        .id_name = config.id_name().c_str(),
        .length = config.length(),
        .use_upper = config.use_upper(),
        .use_lower = config.use_lower(),
        .alphabet = config.alphabet(),
        .use_special_chars = config.use_special_chars(),
        .special_chars_count = config.special_chars_count(),
        .special_charset = config.special_charset().c_str(),
        .use_digits = config.use_digits(),
        .digits_amount = config.digits_amount()
    };
}

KEYSAVER_API(keysaverInit, keysaverString configPath) {
    if (!configPath)
        return KeysaverStatus::E_INVALID_ARG;

    try {
        ks_impl = &Keysaver::Engine::Get(configPath);
    }
    catch (KeysaverStatus code) {
        return code;
    }
    catch (...) {
        return KeysaverStatus::E_UNEXPECTED_EXCEPTION;
    }

    return ks_impl->FirstUsage();
}

KEYSAVER_API(keysaverClose) {
    if (!ks_impl)
        return KeysaverStatus::S_OK;

    ks_impl->Invalidate();
    ks_impl = nullptr;
    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverSetMasterPassword, keysaverString masterPassword){
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    if (!masterPassword)
        return KeysaverStatus::E_INVALID_ARG;

    return ks_impl->SetMasterPassword(masterPassword);
}

KEYSAVER_API(keysaverAddService, keysaverService service) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    KeysaverConfig::Service servConf = ConvertService(service);
    return ks_impl->AddService(servConf);
}

KEYSAVER_API(keysaverDeleteService, keysaverString serviceName) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;
    if (!serviceName)
        return KeysaverStatus::E_INVALID_ARG;

    return ks_impl->DeleteService(serviceName);
}

KEYSAVER_API(keysaverEditService, keysaverString oldServiceName, keysaverService newService) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;
    if (!oldServiceName)
        return KeysaverStatus::E_INVALID_ARG;

    KeysaverConfig::Service servConf = ConvertService(newService);
    return ks_impl->EditService(oldServiceName, servConf);
}

KEYSAVER_API(keysaverAddConfiguration, keysaverConfig confDescr) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    KeysaverConfig::Configuration config = 
        ConvertConfiguration(confDescr);
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

    *servicesCount = services.size();
    return code;
}

KEYSAVER_API(keysaverGetServicesList, keysaverServicesList servicesList) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    if (!servicesList)
        return KeysaverStatus::E_INVALID_ARG;

    std::list<std::string> services_list;
    auto code = ks_impl->GetServicesList(&services_list);
    if (is_keysaver_error(code))
        return code;

    int index = 0;
    for (auto& service: services_list) {
        if (!servicesList[index])
            return KeysaverStatus::E_INVALID_ARG;
        if (service.size() > KEYSAVER_STRING_MAX_SIZE)
            return KeysaverStatus::E_TOO_LONG_STRING;

        std::strcpy(servicesList[index], service.c_str());
        index++;
    }

    return code;
}

KEYSAVER_API(keysaverGetService, keysaverString serviceName, keysaverServiceRef service) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;
    if (!serviceName || !service)
        return KeysaverStatus::E_INVALID_ARG;

    KeysaverConfig::Service c_service;
    auto code = ks_impl->GetService(serviceName, &c_service);
    if (is_keysaver_error(code)) {
        return code;
    }

    *service = ConvertService(c_service);
    return code;
}

KEYSAVER_API(keysaverGetConfigurationsCount, keysaverIntRef configurationsCount) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    if (!configurationsCount)
        return KeysaverStatus::E_INVALID_ARG;

    Keysaver::DBManager::ConfigurationsNames configs;
    auto code = ks_impl->GetConfigurationsList(&configs);
    if (is_keysaver_error(code))
        return code;

    *configurationsCount = configs.size();
    return code;
}

KEYSAVER_API(keysaverGetConfigurationsList, keysaverConfigsList configurationsList) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    if (!configurationsList)
        return KeysaverStatus::E_INVALID_ARG;

    std::list<std::string> c_conf_list;
    auto code = ks_impl->GetConfigurationsList(&c_conf_list);
    if (is_keysaver_error(code))
        return code;

    int index = 0;
    for (auto& config: c_conf_list) {
        if (!configurationsList[index])
            return KeysaverStatus::E_INVALID_ARG;
        if (config.size() > KEYSAVER_STRING_MAX_SIZE)
            return KeysaverStatus::E_TOO_LONG_STRING;

        std::strcpy(configurationsList[index], config.c_str());
        index++;
    }

    return code;
}

KEYSAVER_API(keysaverGetAlphabetsCount, keysaverIntRef alphabetsCount) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    if (!alphabetsCount)
        return KeysaverStatus::E_INVALID_ARG;

    size_t alphabets_cnt = Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS.size();
    *alphabetsCount = alphabets_cnt;
    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverGetAlphabetsList, keysaverAlphabetList alphabetsList) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    if (!alphabetsList)
        return KeysaverStatus::E_INVALID_ARG;

    int index = 0;
    for (auto& alphabet: Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS) {
        if (!alphabetsList[index])
            return KeysaverStatus::E_INVALID_ARG;
        std::string tmpAlphabetName{alphabet.begin(), alphabet.end()};
        if (tmpAlphabetName.size() > KEYSAVER_STRING_MAX_SIZE)
            return KeysaverStatus::E_TOO_LONG_STRING;

        std::memcpy(alphabetsList[index], tmpAlphabetName.c_str(), tmpAlphabetName.length());
        index++;
    }

    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverGetDatabaseName, keysaverStringRef fileName) {
    assert(Keysaver::DBManager::DB_NAME.size() <= KEYSAVER_STRING_MAX_SIZE);

    if (!fileName)
        return KeysaverStatus::E_INVALID_ARG;

    std::string c_file_name = Keysaver::DBManager::DB_NAME;
    std::strcpy(fileName, c_file_name.c_str());
    return KeysaverStatus::S_OK;
}

KEYSAVER_API(keysaverGeneratePassword,
             keysaverString serviceName, keysaverInt imageIndex, keysaverStringRef result) {
    if (!ks_impl)
        return KeysaverStatus::E_NOT_INITIALIZED;

    if (!serviceName || !result)
        return KeysaverStatus::E_INVALID_ARG;

    std::u8string cpp_result;
    auto code = ks_impl->GeneratePassword(
        serviceName, imageIndex, &cpp_result);
    if (is_keysaver_error(code)) {
        return code;
    }

    std::string c_result{cpp_result.begin(), cpp_result.end()};
    if (c_result.size() > KEYSAVER_STRING_MAX_SIZE)
        return KeysaverStatus::E_TOO_LONG_STRING;

    std::memcpy(result, c_result.c_str(), c_result.size());
    return code;
}

