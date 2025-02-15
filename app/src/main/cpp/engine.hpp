// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "keysaver/interface.h"

#include "db_manager.hpp"
#include "crypto_provider.hpp"
#include "password_generator.hpp"

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

        // methods
        KeysaverStatus SetMasterPassword(const std::string& masterPassword);
        KeysaverStatus AddService(const KeysaverConfig::Service& service);
        KeysaverStatus DeleteService(const std::string& serviceName);
        KeysaverStatus EditService(const std::string& serviceName,
                                   const KeysaverConfig::Service& service);
        KeysaverStatus AddConfiguration(const KeysaverConfig::Configuration& config);
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

        KeysaverStatus GeneratePassword(const std::string& serviceName,
                                        size_t saltNumber,
                                        std::u8string* result);

        KeysaverStatus Invalidate();

    private:
        explicit Engine(const std::string& pathToDB):
            m_db(pathToDB) {}

        // types
        using uint8_t = std::uint8_t;

        // consts
        static constexpr size_t MIN_PASSWORD_LEN = 8;
        static constexpr size_t MAX_PASSWORD_LEN = 30;

        // members
        DBManager            m_db;
        PasswordGenerator    m_generator{};
        mutable HashProvider m_hasher{};
    };
}