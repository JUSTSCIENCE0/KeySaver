// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "keysaver/interface.h"
#include "configuration.pb.h"

#include <openssl/types.h>

#include <list>
#include <string>
#include <cstdint>

namespace Keysaver {
    class Implementation final {
    public:
        Implementation(const Implementation&) = delete;
        Implementation(Implementation&&) = delete;
        Implementation& operator=(const Implementation&) = delete;
        Implementation& operator=(Implementation&&) = delete;
        ~Implementation();

        static Implementation& Get() {
            static Implementation impl;
            return impl;
        }

        KeysaverStatus Init(const std::string& configPath);

        KeysaverStatus SetMasterPassword(const std::string& masterPassword);

        KeysaverStatus GetServicesCount(size_t* count) const;
        KeysaverStatus GetServicesList(std::list<std::string>* serviceNames) const;
        KeysaverStatus GetConfigurationsCount(size_t* count) const;
        KeysaverStatus GetConfigurationsList(std::list<std::string>* configNames) const;

    private:
        // types
        using uint8_t = std::uint8_t;
        enum class HASH_USAGE { E_ENCRYPTION, E_SALT };

        // methods
        Implementation() = default;
        KeysaverStatus CalculateHash(const std::string& masterPassword, HASH_USAGE usage);

        // consts
        static constexpr auto   CONFIG_NAME = "/config.bin";
        static constexpr size_t MIN_PASSWORD_LEN = 8;
        static constexpr size_t HASH_SIZE = 32;

        // flags
        bool m_isInited = false;
        bool m_isFirstUsing = false;

        //members
        std::array<uint8_t, HASH_SIZE> m_encryption_hash{};
        std::array<uint8_t, HASH_SIZE> m_salt_hash{};

        //openssl
        EVP_MD_CTX* m_ossl_ctx = nullptr;

        KeysaverConfig::DataBase m_db{};
    };
}