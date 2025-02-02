// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "crypto_provider.hpp"

#include "keysaver/interface.h"
#include "configuration.pb.h"

#include <array>
#include <string>
#include <filesystem>
#include <cstdint>

namespace Keysaver {
    class DBManager final {
    public:
        // consts
        static constexpr size_t      ENCRYPTION_KEY_SIZE = 32;
        static constexpr std::string DEFAULT_CONFIG_NAME = "Default";

        // types
        using EncryptionKey = std::array<uint8_t, ENCRYPTION_KEY_SIZE>;

        // ctors/dtor
        explicit DBManager(const std::string& pathToDB);
        ~DBManager();

        DBManager(const DBManager&) = delete;
        DBManager(DBManager&&) = delete;
        DBManager& operator=(const DBManager&) = delete;
        DBManager& operator=(DBManager&&) = delete;

        // interface
        bool IsFileExists() const { return std::filesystem::exists(m_db_path); }
        bool IsServiceExists(const std::string& serviceName) const;
        bool IsServiceUrlExists(const std::string& serviceUrl) const;
        bool IsConfigExists(const std::string& configName) const;

        const KeysaverConfig::Service* GetService(const std::string& serviceName) const;

        KeysaverStatus SetEncryptionKey(const EncryptionKey& key);
        KeysaverStatus Flush() const;

        const KeysaverConfig::DataBase& Get() const { return m_proto_db; }
        KeysaverConfig::DataBase& Patch() {
            m_is_db_modified = true;
            return m_proto_db;
        }

    private:
        // types
        using uint8_t = std::uint8_t;

        // consts
        static constexpr auto        DB_NAME = "/database.bin";

        // methods
        KeysaverStatus Read();

        //members
        mutable bool             m_is_db_modified = false;
        KeysaverConfig::DataBase m_proto_db{};
        std::filesystem::path    m_db_path{};
        EncryptionKey            m_encryption_key{};
        CryptoProvider&          m_crypto = CryptoProvider::Get();
    };
} // Keysaver

