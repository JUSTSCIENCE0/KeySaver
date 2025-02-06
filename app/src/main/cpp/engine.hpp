// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "keysaver/interface.h"

#include "db_manager.hpp"
#include "crypto_provider.hpp"

#include <list>
#include <string>
#include <cstdint>
#include <shared_mutex>
#include <string_view>

namespace Keysaver {
    class Engine final {
    public:

        // ctors & dtor
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;
        ~Engine() = default;

        static Engine& Get(const std::string& pathToDB) {
            static Engine impl(pathToDB);
            return impl;
        }

        // consts
        static constexpr std::array<std::u8string_view, 2> SUPPORTED_ALPHABETS = {
                u8"Latin (English, US)", u8"Кириллица (Русский, Россия)"
        };

        // methods
        KeysaverStatus SetMasterPassword(const std::string& masterPassword);
        KeysaverStatus AddService(const KeysaverConfig::Service& service);
        KeysaverStatus DeleteService(const std::string& serviceName);
        KeysaverStatus EditService(const std::string& serviceName,
                                   const KeysaverConfig::Service& service);
        KeysaverStatus SyncDatabase() const;

        KeysaverStatus FirstUsage() const {
            return m_db.IsFileExists() ?
                    KeysaverStatus::S_OK :
                    KeysaverStatus::M_DATABASE_NOT_FOUND;
        }
        KeysaverStatus GetServicesCount(size_t* count) const;
        KeysaverStatus GetServicesList(std::list<std::string>* serviceNames) const;
        KeysaverStatus GetService(const std::string& serviceName,
                                  const KeysaverConfig::Service** service) const;
        KeysaverStatus GetConfigurationsCount(size_t* count) const;
        KeysaverStatus GetConfigurationsList(std::list<std::string>* configNames) const;

        KeysaverStatus Invalidate();

    private:
        explicit Engine(const std::string& pathToDB):
            m_db(pathToDB) {}

        // types
        using uint8_t = std::uint8_t;
        enum class HASH_USAGE { E_ENCRYPTION, E_SALT };

        // consts
        static constexpr size_t      MIN_PASSWORD_LEN = 8;

        //members
        mutable std::mutex m_db_mutex{};
        DBManager          m_db;

        CryptoProvider& m_crypto = CryptoProvider::Get();

        std::array<uint8_t, CryptoProvider::HASH_SIZE> m_salt_hash{};
    };
}