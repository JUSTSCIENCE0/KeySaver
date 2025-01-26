// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "implementation.hpp"

#include <openssl/evp.h>

#include <filesystem>
#include <vector>
#include <cstdint>

namespace Keysaver {
    Implementation::~Implementation() {
        if (m_ossl_ctx)
            EVP_MD_CTX_free(m_ossl_ctx);
        m_ossl_ctx = nullptr;
    }

    KeysaverStatus Implementation::Init(const std::string& configPath) {
        const std::string configName = configPath + CONFIG_NAME;

        m_ossl_ctx = EVP_MD_CTX_new();
        if (!m_ossl_ctx) return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;
        m_isInited = true;

        if (std::filesystem::exists(configName)) {
            // TODO:
            //   open & read encrypted config
            //   check if it isn't empty or corrupted

            return KeysaverStatus::S_OK;
        }
        else {
            // TODO: create empty unencrypted config

            m_isFirstUsing = true;
            return KeysaverStatus::M_DATABASE_NOT_FOUND;
        }
    }

    KeysaverStatus Implementation::SetMasterPassword(const std::string& masterPassword) {
        if (!m_isInited) return KeysaverStatus::E_NOT_INITIALIZED;
        if (masterPassword.length() < MIN_PASSWORD_LEN)
            return KeysaverStatus::E_TOO_SHORT_MASTER_PASSWORD;

        auto code = KeysaverStatus::S_OK;
        code = CalculateHash(masterPassword, HASH_USAGE::E_ENCRYPTION);
        if (is_keysaver_error(code)) return code;
        code = CalculateHash(masterPassword, HASH_USAGE::E_SALT);
        if (is_keysaver_error(code)) return code;

        // TODO:
        //  if not m_isFirstUsing
        //    decrypt config
        //    validate config
        //  else
        //    encrypt empty unencrypted config
        //    save encrypted config

        return code;
    }

    KeysaverStatus Implementation::AddService(const KeysaverConfig::Service& service) {
        if (IsServiceExists(service.name()) == KeysaverStatus::S_IS_FOUND)
            return KeysaverStatus::E_SERVICE_ALREADY_EXISTS;

        if (IsServiceUrlExists(service.url()) == KeysaverStatus::S_IS_FOUND)
            return KeysaverStatus::E_SERVICE_URL_ALREADY_EXISTS;

        if (IsConfigExists(service.conf_id()) == KeysaverStatus::S_IS_FOUND)
            return KeysaverStatus::E_CONFIG_NOT_EXISTS;

        auto new_service = m_db.add_services();
        *new_service = service;

        // TODO: rewrite db

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Implementation::GetServicesCount(size_t* count) const {
        if (!count) return KeysaverStatus::E_INVALID_ARG;

        *count = m_db.services_size();
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Implementation::GetServicesList(std::list<std::string>* serviceNames) const {
        if (!serviceNames) return KeysaverStatus::E_INVALID_ARG;

        for (const auto& service: m_db.services()) {
            serviceNames->push_back(service.name());
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Implementation::GetConfigurationsCount(size_t* count) const {
        if (!count) return KeysaverStatus::E_INVALID_ARG;

        *count = m_db.configurations_size() + 1;
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Implementation::GetConfigurationsList(
            std::list<std::string>* configNames) const {
        if (!configNames) return KeysaverStatus::E_INVALID_ARG;

        configNames->emplace_back(DEFAULT_CONFIG_NAME);
        for (const auto& config: m_db.configurations()) {
            configNames->push_back(config.id_name());
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Implementation::CalculateHash(
            const std::string& masterPassword,
            HASH_USAGE usage) {
        uint8_t* hash_pntr = nullptr;
        const EVP_MD* md   = nullptr;
        switch (usage) {
            case HASH_USAGE::E_ENCRYPTION:
                hash_pntr = m_encryption_hash.data();
                md = EVP_sha3_256();
                break;
            case HASH_USAGE::E_SALT:
                hash_pntr = m_salt_hash.data();
                md = EVP_blake2s256();
                break;
            default:
                assert(!"unexpected behavior");
                break;
        }

        if (!md) return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        if (EVP_DigestInit_ex(m_ossl_ctx, md, nullptr) != 1)
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        if (EVP_DigestUpdate(
                m_ossl_ctx,
                masterPassword.data(),
                masterPassword.size()) != 1)
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        unsigned int hash_len = 0;
        if (EVP_DigestFinal_ex(m_ossl_ctx, hash_pntr, &hash_len) != 1)
            return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;
        assert(hash_len == HASH_SIZE);

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Implementation::IsServiceExists(const std::string& serviceName) const {
        const auto& services = m_db.services();

        auto result = std::find_if(
                services.begin(),
                services.end(),
                [&serviceName](const auto& service){
                    return service.name() == serviceName;
                });

        return (result != services.end()) ?
            KeysaverStatus::S_IS_FOUND :
            KeysaverStatus::E_SERVICE_NOT_EXISTS;
    }

    KeysaverStatus Implementation::IsServiceUrlExists(const std::string& serviceUrl) const {
        const auto& services = m_db.services();

        auto result = std::find_if(
                services.begin(),
                services.end(),
                [&serviceUrl](const auto& service){
                    return service.url() == serviceUrl;
                });

        return (result != services.end()) ?
               KeysaverStatus::S_IS_FOUND :
               KeysaverStatus::E_SERVICE_NOT_EXISTS;
    }

    KeysaverStatus Implementation::IsConfigExists(const std::string& configName) const {
        const auto& configs = m_db.configurations();

        auto result = std::find_if(
                configs.begin(),
                configs.end(),
                [&configName](const auto& config){
                   return config.id_name() == configName;
                });

        return (result != configs.end()) ?
               KeysaverStatus::S_IS_FOUND :
               KeysaverStatus::E_CONFIG_NOT_EXISTS;
    }
}