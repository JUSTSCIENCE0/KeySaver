// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "db_manager.hpp"
#include <fstream>

namespace Keysaver {
    DBManager::DBManager(const std::string& pathToDB) :
        m_db_path(pathToDB + DB_NAME) {
        assert(!pathToDB.empty());
    }

    DBManager::~DBManager() {
        Flush();
    }

    const KeysaverConfig::Service* DBManager::GetService(const std::string& serviceName) const {
        const auto& services = m_proto_db.services();

        auto result = std::find_if(
                services.begin(),
                services.end(),
                [&serviceName](const auto& service){
                    return service.name() == serviceName;
                });

        return (result != services.end()) ? result.operator->() : nullptr;
    }

    int DBManager::GetServiceIndex(const std::string& serviceName) const {
        const auto& services = m_proto_db.services();

        for (int i = 0; i < services.size(); i++) {
            if (services.Get(i).name() == serviceName) return i;
        }

        return -1;
    }

    bool DBManager::IsServiceExists(const std::string& serviceName) const {
        return (GetService(serviceName) != nullptr);
    }

    bool DBManager::IsServiceUrlExists(const std::string& serviceUrl) const {
        const auto& services = m_proto_db.services();

        auto result = std::find_if(
                services.begin(),
                services.end(),
                [&serviceUrl](const auto& service){
                    return service.url() == serviceUrl;
                });

        return (result != services.end());
    }

    bool DBManager::IsConfigExists(const std::string& configName) const {
        const auto& configs = m_proto_db.configurations();

        auto result = std::find_if(
                configs.begin(),
                configs.end(),
                [&configName](const auto& config){
                    return config.id_name() == configName;
                });

        return (result != configs.end());
    }

    KeysaverStatus DBManager::SetEncryptionKey(const EncryptionKey& key) {
        m_encryption_key = key;
        return Read();
    }

    KeysaverStatus DBManager::Flush() const {
        if (!m_is_db_modified) return KeysaverStatus::S_OK;

        std::vector<uint8_t> binary_db(m_proto_db.ByteSizeLong());
        if (!m_proto_db.SerializeToArray(
                binary_db.data(), static_cast<int>(binary_db.size())))
            return KeysaverStatus::E_DB_CORRUPTED;

        // TODO:
        //  calculate & write checksum
        //  encrypt binary_db

        std::ofstream db_file(m_db_path, std::ios::binary);
        if (!db_file) return KeysaverStatus::E_DB_WRITE_ERROR;

        if (!db_file.write(
                reinterpret_cast<const char *>(binary_db.data()),
                binary_db.size()))
            return KeysaverStatus::E_DB_WRITE_ERROR;

        db_file.close();

        m_is_db_modified = false;
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus DBManager::Read() {
        if (!IsFileExists()) return KeysaverStatus::S_OK;

        std::ifstream db_file(m_db_path, std::ios::binary | std::ios::ate);
        if (!db_file) return KeysaverStatus::E_DB_READ_ERROR;

        auto db_size = db_file.tellg();
        if (!db_size) return KeysaverStatus::E_DB_CORRUPTED;
        std::vector<uint8_t> binary_db(db_size);

        db_file.seekg(0, std::ios::beg);
        if (!db_file.read(reinterpret_cast<char *>(binary_db.data()), db_size))
            return KeysaverStatus::E_DB_READ_ERROR;

        db_file.close();

        // TODO:
        //  decrypt binary_db
        //  check checksum

        if (!m_proto_db.ParseFromArray(
                binary_db.data(), static_cast<int>(binary_db.size())))
            return KeysaverStatus::E_DB_CORRUPTED;

        m_is_db_modified = false;
        return KeysaverStatus::S_OK;
    }
} // Keysaver