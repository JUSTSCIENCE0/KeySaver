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
    class Engine final {
    public:
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;
        ~Engine();

        static Engine& Get() {
            static Engine impl;
            return impl;
        }

        KeysaverStatus Init(const std::string& configPath);

        KeysaverStatus SetMasterPassword(const std::string& masterPassword);
        KeysaverStatus AddService(const KeysaverConfig::Service& service);

        KeysaverStatus GetServicesCount(size_t* count) const;
        KeysaverStatus GetServicesList(std::list<std::string>* serviceNames) const;
        KeysaverStatus GetConfigurationsCount(size_t* count) const;
        KeysaverStatus GetConfigurationsList(std::list<std::string>* configNames) const;

    private:
        // types
        using uint8_t = std::uint8_t;
        enum class HASH_USAGE { E_ENCRYPTION, E_SALT };

        // methods
        Engine() = default;
        KeysaverStatus CalculateHash(const std::string& masterPassword, HASH_USAGE usage);
        KeysaverStatus ReadDB();
        KeysaverStatus RewriteDB() const;
        KeysaverStatus IsServiceExists(const std::string& serviceName) const;
        KeysaverStatus IsServiceUrlExists(const std::string& serviceUrl) const;
        KeysaverStatus IsConfigExists(const std::string& configName) const;

        // consts
        static constexpr auto        CONFIG_NAME = "/config.bin";
        static constexpr size_t      MIN_PASSWORD_LEN = 8;
        static constexpr size_t      HASH_SIZE = 32;
        static constexpr std::string DEFAULT_CONFIG_NAME = "Default";

        // flags
        bool m_isInited = false;
        bool m_isFirstUsing = false;

        //members
        KeysaverConfig::DataBase m_db{};
        std::array<uint8_t, HASH_SIZE> m_encryption_hash{};
        std::array<uint8_t, HASH_SIZE> m_salt_hash{};
        std::filesystem::path m_db_path{};

        //openssl
        EVP_MD_CTX* m_ossl_ctx = nullptr;
    };
}