// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "implementation.hpp"

#include <filesystem>
#include <vector>
#include <cstdint>

namespace Keysaver {
    using std::uint8_t;

    static constexpr auto CONFIG_NAME = "/config.bin";

    static bool m_isInited = false;
    static bool m_isFirstUsing = false;

    KeysaverStatus init(const std::string& configPath) {
        const std::string configName = configPath + CONFIG_NAME;

        if (std::filesystem::exists(configName)) {
            // TODO:
            //   open & read encrypted config
            //   check if it isn't empty or corrupted

            m_isInited = true;
            return KeysaverStatus::S_OK;
        }
        else {
            // TODO: create empty unencrypted config

            m_isFirstUsing = true;
            m_isInited = true;
            return KeysaverStatus::M_CONFIG_NOT_FOUND;
        }
    }

    KeysaverStatus set_master_password(const std::string& masterPassword) {
        if (!m_isInited) return KeysaverStatus::E_NOT_INITIALIZED;

        // TODO:
        //  calculate password hashes
        //  if not m_isFirstUsing
        //    decrypt config
        //    validate config
        //  else
        //    encrypt empty unencrypted config
        //    save encrypted config

        return KeysaverStatus::S_OK;
    }
}