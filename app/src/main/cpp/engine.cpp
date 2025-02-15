// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "engine.hpp"

#include <openssl/evp.h>

#include <filesystem>
#include <vector>
#include <cstdint>
#include <fstream>

namespace Keysaver {
    KeysaverStatus ConstructPassword(
            const KeysaverConfig::Configuration& config,
            PRNGProvider& prng,
            std::string* result) {
        assert(result);

        return KeysaverStatus::E_NOT_IMPLEMENTED;
    }

    KeysaverStatus Engine::SetMasterPassword(const std::string& masterPassword) {
        if (masterPassword.length() < MIN_PASSWORD_LEN)
            return KeysaverStatus::E_TOO_SHORT_MASTER_PASSWORD;

        PRNGProvider::PRNGKey db_key{};
        if (!m_hasher.CalculateHash(
                masterPassword.data(),
                masterPassword.size(),
                HashProvider::HashAlgorithm::SHA3_256,
                &db_key))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        HashProvider::Hash db_iv{};
        if (!m_hasher.CalculateHash(
                masterPassword.data(),
                masterPassword.size(),
                HashProvider::HashAlgorithm::SHA_256,
                &db_iv))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        PRNGProvider::PRNGKey prng_key{};
        if (!m_hasher.CalculateHash(
                masterPassword.data(),
                masterPassword.size(),
                HashProvider::HashAlgorithm::BLAKE2_256,
                &prng_key))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        auto code = m_generator.SetKey(prng_key);
        if (is_keysaver_error(code)) return code;

        code = m_db.SetEncryptionParams(db_key, db_iv);
        if (is_keysaver_error(code)) return code;

        return code;
    }

    KeysaverStatus Engine::SyncDatabase() const {
        return m_db.Flush();
    }

    KeysaverStatus Engine::AddService(const KeysaverConfig::Service& service) {
        return m_db.AddService(service);
    }

    KeysaverStatus Engine::EditService(const std::string& serviceName,
                                       const KeysaverConfig::Service& newService) {
        return m_db.EditService(serviceName, newService);
    }

    KeysaverStatus Engine::DeleteService(const std::string& serviceName) {
        return m_db.DeleteService(serviceName);
    }

    KeysaverStatus Engine::AddConfiguration(const KeysaverConfig::Configuration& config) {
        // validate config
        if (config.length() < MIN_PASSWORD_LEN ||
            config.length() > MAX_PASSWORD_LEN)
            return KeysaverStatus::E_INVALID_PASSWORD_LENGTH;

        if (size_t(config.alphabet()) >= Keysaver::PasswordGenerator::SUPPORTED_ALPHABETS.size())
            return KeysaverStatus::E_UNSUPPORTED_ALPHABET;

        if (config.special_chars_count() > config.length() / 4)
            return KeysaverStatus::E_INVALID_SPECIAL_CHAR_COUNT;

        if (config.digits_amount() > config.length() / 4)
            return KeysaverStatus::E_INVALID_DIGITS_AMOUNT;

        if (!config.use_lower() && !config.use_upper())
            return KeysaverStatus::E_WITHOUT_ANY_CASE;

        return m_db.AddConfiguration(config);
    }

    KeysaverStatus Engine::GetServicesList(std::list<std::string>* serviceNames) const {
        if (!serviceNames) return KeysaverStatus::E_INVALID_ARG;

        *serviceNames = m_db.GetServicesList();
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetService(const std::string& serviceName,
                                      KeysaverConfig::Service* service) const {
        if (!service) return KeysaverStatus::E_INVALID_ARG;

        if (!m_db.GetService(serviceName, service))
            return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetConfigurationsList(
            std::list<std::string>* configNames) const {
        if (!configNames) return KeysaverStatus::E_INVALID_ARG;

        *configNames = m_db.GetConfigurationsList();
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GeneratePassword(
            const std::string& serviceName, size_t saltNumber, std::u8string* result) {
        if (saltNumber >= SALTS_COUNT || !result)
            return KeysaverStatus::E_INVALID_ARG;

        KeysaverConfig::Service service;
        if (!m_db.GetService(serviceName, &service))
            return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        KeysaverConfig::Configuration config;
        if (!m_db.GetConfiguration(service.conf_id(), &config))
            return KeysaverStatus::E_CONFIG_NOT_EXISTS;

        HashProvider::Hash init_vector{};
        if (!m_hasher.CalculateHash(
                service.url().data(), service.url().size(),
                HashProvider::HashAlgorithm::SHA3_256,
                init_vector.data(), int(saltNumber)))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        return m_generator.ConstructPassword(init_vector, config, result);
    }

    KeysaverStatus Engine::Invalidate() {
        m_generator.Invalidate();
        m_db.Invalidate();
        return KeysaverStatus::S_OK;
    }
}