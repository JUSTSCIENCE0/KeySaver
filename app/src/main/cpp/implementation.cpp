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
            return KeysaverStatus::M_CONFIG_NOT_FOUND;
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

    KeysaverStatus Implementation::GetServicesCount(size_t* count) const {
        if (!count) return KeysaverStatus::E_INVALID_ARG;

        *count = m_db.services_size();
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus Implementation::GetServicesList(std::list<std::string>* serviceNames) const {
        if (!serviceNames) return KeysaverStatus::E_INVALID_ARG;

        for (auto& service: m_db.services()) {
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

        configNames->emplace_back("Default");
        for (auto& config: m_db.configurations()) {
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
}