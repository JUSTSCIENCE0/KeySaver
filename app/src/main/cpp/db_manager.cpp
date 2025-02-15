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

    bool DBManager::GetService(const std::string& serviceName,
                               KeysaverConfig::Service* service) const {
        std::shared_lock lock(m_db_mutex);

        int serviceID = FindService(serviceName);
        if (INVALID_INDEX == serviceID)
            return false;

        *service = m_proto_db.services(serviceID);
        return true;
    }

    int DBManager::GetServiceIndex(const std::string& serviceName) const {
        return FindService(serviceName);
    }

    DBManager::ServicesNames DBManager::GetServicesList() const {
        std::shared_lock lock(m_db_mutex);

        ServicesNames result;
        for (const auto& service: m_proto_db.services()) {
            result.push_back(service.name());
        }

        return result;
    }

    bool DBManager::GetConfiguration(const std::string& configName,
                                     KeysaverConfig::Configuration* config) const {
        std::shared_lock lock(m_db_mutex);

        int configID = FindConfiguration(configName);
        if (INVALID_INDEX == configID)
            return false;
        if (DEFAULT_CONFIG_INDEX == configID) {
            GetDefaultConfiguration(config);
            return true;
        }

        *config = m_proto_db.configurations(configID);
        return true;
    }

    int DBManager::GetConfigurationIndex(const std::string& configName) const {
        return FindConfiguration(configName);
    }

    DBManager::ConfigurationsNames DBManager::GetConfigurationsList() const {
        ConfigurationsNames result;
        result.emplace_back(DBManager::DEFAULT_CONFIG_NAME);

        std::shared_lock lock(m_db_mutex);

        for (const auto& config: m_proto_db.configurations()) {
            result.push_back(config.id_name());
        }

        return result;
    }

    KeysaverStatus DBManager::AddService(const KeysaverConfig::Service& service) {
        std::scoped_lock lock(m_db_mutex);

        if (INVALID_INDEX != FindService(service.name()))
            return KeysaverStatus::E_SERVICE_ALREADY_EXISTS;

        if (IsServiceUrlExists(service.url()))
            return KeysaverStatus::E_SERVICE_URL_ALREADY_EXISTS;

        if (INVALID_INDEX == FindConfiguration(service.conf_id()))
            return KeysaverStatus::E_CONFIG_NOT_EXISTS;

        auto new_service = m_proto_db.add_services();
        *new_service = service;

        m_is_db_modified = true;
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus DBManager::EditService(const std::string& serviceName,
                               const KeysaverConfig::Service& newService) {
        std::scoped_lock lock(m_db_mutex);

        int serviceID = FindService(serviceName);
        if (INVALID_INDEX == serviceID)
            return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        // check new config
        if (INVALID_INDEX == FindConfiguration(newService.conf_id()))
            return KeysaverStatus::E_CONFIG_NOT_EXISTS;

        // check & patch name
        if (serviceName != newService.name()) {
            if (INVALID_INDEX != FindService(newService.name()))
                return KeysaverStatus::E_SERVICE_ALREADY_EXISTS;

            m_proto_db.mutable_services(serviceID)->set_name(newService.name());
            m_is_db_modified = true;
        }

        // patch config
        auto oldServiceConfig = m_proto_db.services(serviceID).conf_id();
        if (oldServiceConfig != newService.conf_id()) {
            m_proto_db.mutable_services(serviceID)->set_conf_id(newService.conf_id());
            m_is_db_modified = true;
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus DBManager::DeleteService(const std::string& serviceName) {
        std::scoped_lock lock(m_db_mutex);

        int servIndex = FindService(serviceName);
        if (INVALID_INDEX == servIndex)
            return KeysaverStatus::E_SERVICE_NOT_EXISTS;

        m_proto_db.mutable_services()->DeleteSubrange(servIndex, 1);

        assert(INVALID_INDEX == FindService(serviceName));
        m_is_db_modified = true;
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus DBManager::AddConfiguration(const KeysaverConfig::Configuration& config) {
        std::scoped_lock lock(m_db_mutex);

        if (INVALID_INDEX != FindConfiguration(config.id_name()))
            return KeysaverStatus::E_SERVICE_ALREADY_EXISTS;

        // add config
        auto new_config = m_proto_db.add_configurations();
        *new_config = config;

        m_is_db_modified = true;
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus DBManager::SetEncryptionKey(const EncryptionKey& key) {
        m_encryption_key = key;
        return Read();
    }

    KeysaverStatus DBManager::Flush() const {
        std::vector<uint8_t> binary_db;

        {
            std::scoped_lock lock(m_db_mutex);

            if (!m_is_db_modified)
                return KeysaverStatus::S_OK;

            binary_db.resize(m_proto_db.ByteSizeLong());
            if (!m_proto_db.SerializeToArray(
                    binary_db.data(), static_cast<int>(binary_db.size())))
                return KeysaverStatus::E_DB_CORRUPTED;

            m_is_db_modified = false;
        }

        // TODO:
        //  calculate & write checksum
        //  encrypt binary_db

        {
            std::scoped_lock lock(m_db_file_mutex);

            std::ofstream db_file(m_db_path, std::ios::binary);
            if (!db_file)
                return KeysaverStatus::E_DB_WRITE_ERROR;

            if (!db_file.write(
                    reinterpret_cast<const char *>(binary_db.data()),
                    binary_db.size()))
                return KeysaverStatus::E_DB_WRITE_ERROR;

            db_file.close();
        }

        return KeysaverStatus::S_OK;
    }

    KeysaverStatus DBManager::Read() {
        if (!IsFileExists()) return KeysaverStatus::S_OK;

        std::vector<uint8_t> binary_db;
        {
            std::scoped_lock lock(m_db_file_mutex);

            std::ifstream db_file(m_db_path, std::ios::binary | std::ios::ate);
            if (!db_file) return KeysaverStatus::E_DB_READ_ERROR;

            auto db_size = db_file.tellg();
            if (!db_size) return KeysaverStatus::E_DB_CORRUPTED;
            binary_db.resize(db_size);

            db_file.seekg(0, std::ios::beg);
            if (!db_file.read(reinterpret_cast<char *>(binary_db.data()), db_size))
                return KeysaverStatus::E_DB_READ_ERROR;

            db_file.close();
        }

        // TODO:
        //  decrypt binary_db
        //  check checksum

        {
            std::scoped_lock lock(m_db_mutex);

            if (!m_proto_db.ParseFromArray(
                    binary_db.data(), static_cast<int>(binary_db.size())))
                return KeysaverStatus::E_DB_CORRUPTED;

            m_is_db_modified = false;
        }
        return KeysaverStatus::S_OK;
    }

    void DBManager::Invalidate() {
        std::memset(m_encryption_key.data(), 0, ENCRYPTION_KEY_SIZE);

        std::scoped_lock lock(m_db_mutex);
        m_proto_db.clear_configurations();
        m_proto_db.clear_services();
        m_is_db_modified = false;
    }

    int DBManager::FindService(const std::string& serviceName) const {
        // private method - lock free

        const auto& services = m_proto_db.services();

        for (int i = 0; i < services.size(); i++) {
            if (services.Get(i).name() == serviceName) return i;
        }

        return INVALID_INDEX;
    }

    int DBManager::FindConfiguration(const std::string& configName) const {
        // private method - lock free

        if (configName == DEFAULT_CONFIG_NAME) return DEFAULT_CONFIG_INDEX;

        const auto& configs = m_proto_db.configurations();

        for (int i = 0; i < configs.size(); i++) {
            if (configs.Get(i).id_name() == configName) return i;
        }

        return INVALID_INDEX;
    }

    bool DBManager::IsServiceUrlExists(const std::string& serviceUrl) const {
        // private method - lock free

        const auto& services = m_proto_db.services();

        auto result = std::find_if(
                services.begin(),
                services.end(),
                [&serviceUrl](const auto& service){
                    return service.url() == serviceUrl;
                });

        return (result != services.end());
    }
} // Keysaver