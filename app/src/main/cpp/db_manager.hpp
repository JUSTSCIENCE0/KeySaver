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
#include <mutex>
#include <shared_mutex>

namespace Keysaver {
    class DBManager final {
    public:
        // types
        using ServicesNames = std::list<std::string>;
        using ConfigurationsNames = std::list<std::string>;

        // ctors/dtor
        explicit DBManager(const std::string& pathToDB);
        ~DBManager();

        DBManager(const DBManager&) = delete;
        DBManager(DBManager&&) = delete;
        DBManager& operator=(const DBManager&) = delete;
        DBManager& operator=(DBManager&&) = delete;

        // interface
        bool IsFileExists() const { return std::filesystem::exists(m_db_path); }

        bool GetService(const std::string& serviceName,
                        KeysaverConfig::Service* service) const;
        int  GetServiceIndex(const std::string& serviceName) const;
        ServicesNames GetServicesList() const;

        bool GetConfiguration(const std::string& configName,
                              KeysaverConfig::Configuration* config) const;
        int  GetConfigurationIndex(const std::string& configName) const;
        ConfigurationsNames GetConfigurationsList() const;

        KeysaverStatus AddService(const KeysaverConfig::Service& service);
        KeysaverStatus EditService(const std::string& serviceName,
                                   const KeysaverConfig::Service& newService);
        KeysaverStatus DeleteService(const std::string& serviceName);

        KeysaverStatus AddConfiguration(const KeysaverConfig::Configuration& config);

        KeysaverStatus SetEncryptionParams(
                const PRNGProvider::PRNGKey& key,
                const HashProvider::Hash& iv);
        KeysaverStatus Flush() const;

        void Invalidate();

    private:
        // types
        using uint8_t = std::uint8_t;

        // consts
        static constexpr std::string DB_NAME = "/database.bin";
        static constexpr std::string DEFAULT_CONFIG_NAME = "Default";
        static constexpr int         DEFAULT_CONFIG_INDEX = std::numeric_limits<int>::min();
        static constexpr int         INVALID_INDEX = -1;

        // methods
        int FindService(const std::string& serviceName) const;
        int FindConfiguration(const std::string& configName) const;
        bool IsServiceUrlExists(const std::string& serviceUrl) const;

        KeysaverStatus Read();

        bool ApplyCipher(std::vector<uint8_t>* data) const;

        static void GetDefaultConfiguration(KeysaverConfig::Configuration* defConfig) {
            defConfig->set_id_name(DEFAULT_CONFIG_NAME);
            defConfig->set_length(16);
            defConfig->set_use_upper(true);
            defConfig->set_use_lower(true);
            defConfig->set_alphabet(KeysaverConfig::Configuration_AlphabetType_LATIN_ENGLISH);
            defConfig->set_use_special_chars(true);
            defConfig->set_special_chars_count(2);
            defConfig->set_special_charset(R"(!@#$%^&*()_-+=/?.,<>'";:[]{})");
            defConfig->set_use_digits(true);
            defConfig->set_digits_amount(2);
        }

        //members
        mutable std::mutex    m_db_file_mutex{}; // protects only file
        std::filesystem::path m_db_path{};

        mutable std::shared_mutex m_db_mutex; // protects proto_db & modified flag
        mutable bool              m_is_db_modified = false;
        KeysaverConfig::DataBase  m_proto_db{};

        mutable HashProvider m_hasher{};
        mutable PRNGProvider m_prng{};
        HashProvider::Hash   m_encryption_iv;
    };
} // Keysaver

