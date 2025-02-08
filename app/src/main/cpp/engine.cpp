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

        DBManager::EncryptionKey key{};
        if (!m_hasher.CalculateHash(
                masterPassword.data(),
                masterPassword.size(),
                HashProvider::HashAlgorithm::SHA3_256,
                &key))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        if (!m_hasher.CalculateHash(
                masterPassword.data(),
                masterPassword.size(),
                HashProvider::HashAlgorithm::BLAKE2_256,
                &m_prng_key))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        std::scoped_lock lock(m_db_mutex);
        auto code = m_db.SetEncryptionKey(key);
        if (is_keysaver_error(code)) return code;

        return code;
    }

    KeysaverStatus Engine::SyncDatabase() const {
        std::scoped_lock lock(m_db_mutex);
        return m_db.Flush();
    }

    KeysaverStatus Engine::AddService(const KeysaverConfig::Service& service) {
        if (m_db.IsServiceExists(service.name()))
            return KeysaverStatus::E_SERVICE_ALREADY_EXISTS;

        if (m_db.IsServiceUrlExists(service.url()))
            return KeysaverStatus::E_SERVICE_URL_ALREADY_EXISTS;

        if (!m_db.IsConfigExists(service.conf_id()))
            return KeysaverStatus::E_CONFIG_NOT_EXISTS;

        std::scoped_lock lock(m_db_mutex);
        auto new_service = m_db.Patch().add_services();
        *new_service = service;

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::EditService(const std::string& serviceName,
                               const KeysaverConfig::Service& newService) {
        int serviceID = m_db.GetServiceIndex(serviceName);
        if (serviceID < 0) return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        // check new config
        if (!m_db.IsConfigExists(newService.conf_id()))
            return KeysaverStatus::E_CONFIG_NOT_EXISTS;

        std::scoped_lock lock(m_db_mutex);

        // check & patch name
        if (serviceName != newService.name()) {
            if (m_db.IsServiceExists(newService.name()))
                return KeysaverStatus::E_SERVICE_ALREADY_EXISTS;

            m_db.Patch().mutable_services(serviceID)->set_name(newService.name());
        }

        // patch config
        auto oldServiceConfig = m_db.Get().services(serviceID).conf_id();
        if (oldServiceConfig != newService.conf_id()) {
            m_db.Patch().mutable_services(serviceID)->set_conf_id(newService.conf_id());
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::DeleteService(const std::string& serviceName) {
        int servIndex = m_db.GetServiceIndex(serviceName);
        if (servIndex < 0) return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        std::scoped_lock lock(m_db_mutex);
        m_db.Patch().mutable_services()->DeleteSubrange(servIndex, 1);

        assert(!m_db.IsServiceExists(serviceName));
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::AddConfiguration(const KeysaverConfig::Configuration& config) {
        // validate config
        if (config.length() < MIN_PASSWORD_LEN ||
            config.length() > MAX_PASSWORD_LEN)
            return KeysaverStatus::E_INVALID_PASSWORD_LENGTH;

        if (size_t(config.alphabet()) >= SUPPORTED_ALPHABETS.size())
            return KeysaverStatus::E_UNSUPPORTED_ALPHABET;

        if (config.special_chars_count() > config.length() / 4)
            return KeysaverStatus::E_INVALID_SPECIAL_CHAR_COUNT;

        if (config.digits_amount() > config.length() / 4)
            return KeysaverStatus::E_INVALID_DIGITS_AMOUNT;

        if (!config.use_lower() && !config.use_upper())
            return KeysaverStatus::E_WITHOUT_ANY_CASE;

        std::scoped_lock lock(m_db_mutex);
        if (m_db.IsConfigExists(config.id_name()))
            return KeysaverStatus::E_SERVICE_ALREADY_EXISTS;

        // add config
        auto new_config = m_db.Patch().add_configurations();
        *new_config = config;

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetServicesCount(size_t* count) const {
        if (!count) return KeysaverStatus::E_INVALID_ARG;

        *count = m_db.Get().services_size();
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetServicesList(std::list<std::string>* serviceNames) const {
        if (!serviceNames) return KeysaverStatus::E_INVALID_ARG;

        for (const auto& service: m_db.Get().services()) {
            serviceNames->push_back(service.name());
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetService(const std::string& serviceName,
                                      const KeysaverConfig::Service** service) const {
        if (!service) return KeysaverStatus::E_INVALID_ARG;

        auto result = m_db.GetService(serviceName);
        if (!result) return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        *service = result;

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetConfigurationsCount(size_t* count) const {
        if (!count) return KeysaverStatus::E_INVALID_ARG;

        *count = m_db.Get().configurations_size() + 1;
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetConfigurationsList(
            std::list<std::string>* configNames) const {
        if (!configNames) return KeysaverStatus::E_INVALID_ARG;

        configNames->emplace_back(DBManager::DEFAULT_CONFIG_NAME);
        for (const auto& config: m_db.Get().configurations()) {
            configNames->push_back(config.id_name());
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GeneratePassword(
            const std::string& serviceName, size_t saltNumber, std::string* result) const {
        if (saltNumber >= SALTS_COUNT || !result)
            return KeysaverStatus::E_INVALID_ARG;

        auto serviceIndex = m_db.GetServiceIndex(serviceName);
        if (serviceIndex == DBManager::INVALID_INDEX)
            return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        const auto& service = m_db.Get().services(serviceIndex);

        auto confIndex = m_db.GetConfigurationIndex(service.conf_id());
        if (confIndex == DBManager::INVALID_INDEX)
            return KeysaverStatus::E_CONFIG_NOT_EXISTS;

        const auto& config = m_db.Get().configurations(confIndex);

        HashProvider::Hash init_vector{};
        if (!m_hasher.CalculateHash(
                service.url().data(), service.url().size(),
                HashProvider::HashAlgorithm::SHA3_256,
                init_vector.data(), int(saltNumber)))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        PRNGProvider prng(m_prng_key, init_vector);
        return ConstructPassword(config, prng, result);
    }

    KeysaverStatus Engine::Invalidate() {
        std::memset(m_prng_key.data(), 0, HashProvider::HASH_SIZE);

        std::scoped_lock lock(m_db_mutex);
        m_db.Invalidate();

        return KeysaverStatus::S_OK;
    }
}