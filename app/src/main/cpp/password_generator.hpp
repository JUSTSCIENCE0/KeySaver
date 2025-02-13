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
        KeysaverStatus SetKey(const PRNGProvider::PRNGKey& key) {
            return m_prng.SetKey(key) ? S_OK : E_INVALID_ORDER;
        }
        KeysaverStatus ConstructPassword(const HashProvider::Hash& init_vector,
                                         const KeysaverConfig::Configuration& config,
                                         std::u8string* result);
        void Invalidate() { m_prng.Invalidate(); }

    private:
        // types
        enum class SymbolType {
            UPPERCASE_LETTER,
            LOWERCASE_LETTER,
            SPECIAL_CHAR,
            DIGIT
        };
        using Mask = std::vector<SymbolType>;

        // methods
        KeysaverStatus MakeMask(const KeysaverConfig::Configuration& config, Mask* mask);
        KeysaverStatus AddSymbol(SymbolType type,
                                 const KeysaverConfig::Configuration& config,
                                 std::u8string* result);

        // members
        PRNGProvider m_prng;
    };
}
