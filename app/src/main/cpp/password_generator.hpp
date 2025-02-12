// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "configuration.pb.h"
#include "crypto_provider.hpp"

#include <string>
#include <vector>

namespace Keysaver {
    class PasswordGenerator final {
    public:
        // consts
        static constexpr size_t SUPPORTED_ALPHABETS_COUNT = 2;
        static constexpr std::array<std::u8string_view, SUPPORTED_ALPHABETS_COUNT>
            SUPPORTED_ALPHABETS = {
                u8"Latin (English, US)", u8"Кириллица (Русский, Россия)"
        };

        // interface
        PRNGProvider::PRNGKey& PatchKey();
        void Invalidate();
        KeysaverStatus ConstructPassword(const HashProvider::Hash& init_vector,
                                         const KeysaverConfig::Configuration& config,
                                         std::u8string* result) const;
    private:
        // members
        PRNGProvider::PRNGKey m_prng_key;
    };
}
