// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "keysaver/interface.h"

#include <string>
#include <cstdint>

namespace Keysaver {
    class Implementation {
    public:
        Implementation(const Implementation&) = delete;
        Implementation(Implementation&&) = delete;
        Implementation& operator=(const Implementation&) = delete;
        Implementation& operator=(Implementation&&) = delete;

        static Implementation& Get() {
            static Implementation impl;
            return impl;
        }

        KeysaverStatus Init(const std::string& configPath);
        KeysaverStatus SetMasterPassword(const std::string& masterPassword);

    private:
        Implementation() = default;

        using uint8_t = std::uint8_t;

        static constexpr auto CONFIG_NAME = "/config.bin";

        bool m_isInited = false;
        bool m_isFirstUsing = false;
    };
}