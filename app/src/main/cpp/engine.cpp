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
    KeysaverStatus Engine::SetMasterPassword(const std::string& masterPassword) {
        if (masterPassword.length() < MIN_PASSWORD_LEN)
            return KeysaverStatus::E_TOO_SHORT_MASTER_PASSWORD;

        DBManager::EncryptionKey key{};
        if (!m_crypto.CalculateHash(
                masterPassword.data(),
                masterPassword.size(),
                CryptoProvider::HashAlgorithm::SHA3_256,
                &key))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        if (!m_crypto.CalculateHash(
                masterPassword.data(),
                masterPassword.size(),
                CryptoProvider::HashAlgorithm::BLAKE2_256,
                &m_salt_hash))
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        std::unique_lock lock(m_db_mutex);
        auto code = m_db.SetEncryptionKey(key);
        if (is_keysaver_error(code)) return code;

        return code;
    }

    KeysaverStatus Engine::SyncDatabase() const {
        std::unique_lock lock(m_db_mutex);
        return m_db.Flush();
    }

    KeysaverStatus Engine::AddService(const KeysaverConfig::Service& service) {
        {
            std::shared_lock lock(m_db_mutex);
            if (m_db.IsServiceExists(service.name()))
                return KeysaverStatus::E_SERVICE_ALREADY_EXISTS;

            if (m_db.IsServiceUrlExists(service.url()))
                return KeysaverStatus::E_SERVICE_URL_ALREADY_EXISTS;

            if (m_db.IsConfigExists(service.conf_id()))
                return KeysaverStatus::E_CONFIG_NOT_EXISTS;
        }

        {
            std::unique_lock lock(m_db_mutex);
            auto new_service = m_db.Patch().add_services();
            *new_service = service;
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetServicesCount(size_t* count) const {
        if (!count) return KeysaverStatus::E_INVALID_ARG;

        std::shared_lock lock(m_db_mutex);
        *count = m_db.Get().services_size();
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetServicesList(std::list<std::string>* serviceNames) const {
        if (!serviceNames) return KeysaverStatus::E_INVALID_ARG;

        std::shared_lock lock(m_db_mutex);
        for (const auto& service: m_db.Get().services()) {
            serviceNames->push_back(service.name());
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetService(const std::string& serviceName,
                                      const KeysaverConfig::Service** service) const {
        if (!service) return KeysaverStatus::E_INVALID_ARG;

        std::shared_lock lock(m_db_mutex);
        auto result = m_db.GetService(serviceName);
        if (!result) return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        *service = result;

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetConfigurationsCount(size_t* count) const {
        if (!count) return KeysaverStatus::E_INVALID_ARG;

        std::shared_lock lock(m_db_mutex);
        *count = m_db.Get().configurations_size() + 1;
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::GetConfigurationsList(
            std::list<std::string>* configNames) const {
        if (!configNames) return KeysaverStatus::E_INVALID_ARG;

        std::shared_lock lock(m_db_mutex);
        configNames->emplace_back(DBManager::DEFAULT_CONFIG_NAME);
        for (const auto& config: m_db.Get().configurations()) {
            configNames->push_back(config.id_name());
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Engine::Invalidate() {
        std::unique_lock lock(m_db_mutex);

        std::memset(m_salt_hash.data(), 0, CryptoProvider::HASH_SIZE);
        DBManager::EncryptionKey key{};
        m_db.SetEncryptionKey(key);

        m_db.Patch().clear_configurations();
        m_db.Patch().clear_services();

        return KeysaverStatus::S_OK;
    }
}