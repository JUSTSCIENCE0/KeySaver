// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "password_generator.hpp"

namespace Keysaver {
    // types
    struct Alphabet {
        std::u8string_view name;
        std::vector<std::u8string_view> lower_case;
        std::vector<std::u8string_view> upper_case;
    };

    // consts
    static const std::array<Alphabet, PasswordGenerator::SUPPORTED_ALPHABETS_COUNT> LETTERS{
        Alphabet{
             PasswordGenerator::SUPPORTED_ALPHABETS[0],
             {
                 u8"a", u8"b", u8"c", u8"d", u8"e",
                 u8"f", u8"g", u8"h", u8"i", u8"j",
                 u8"k", u8"l", u8"m", u8"n", u8"o",
                 u8"p", u8"q", u8"r", u8"s", u8"t",
                 u8"u", u8"v", u8"w", u8"x", u8"y", u8"z"
             },
             {
                 u8"A", u8"B", u8"C", u8"D", u8"E",
                 u8"F", u8"G", u8"H", u8"I", u8"J",
                 u8"K", u8"L", u8"M", u8"N", u8"O",
                 u8"P", u8"Q", u8"R", u8"S", u8"T",
                 u8"U", u8"V", u8"W", u8"X", u8"Y", u8"Z"
             }
        },
        Alphabet{
            PasswordGenerator::SUPPORTED_ALPHABETS[1],
            {
                u8"а", u8"б", u8"в", u8"г", u8"д", u8"е", u8"ё",
                u8"ж", u8"з", u8"и", u8"й", u8"к", u8"л", u8"м",
                u8"н", u8"о", u8"п", u8"р", u8"с", u8"т", u8"у",
                u8"ф", u8"х", u8"ц", u8"ч", u8"ш", u8"щ", u8"ъ",
                u8"ы", u8"ь", u8"э", u8"ю", u8"я"
            },
            {
                u8"А", u8"Б", u8"В", u8"Г", u8"Д", u8"Е", u8"Ё",
                u8"Ж", u8"З", u8"И", u8"Й", u8"К", u8"Л", u8"М",
                u8"Н", u8"О", u8"П", u8"Р", u8"С", u8"Т", u8"У",
                u8"Ф", u8"Х", u8"Ц", u8"Ч", u8"Ш", u8"Щ", u8"Ъ",
                u8"Ы", u8"Ь", u8"Э", u8"Ю", u8"Я"
            }
        }
    };

    KeysaverStatus PasswordGenerator::MakeMask(
            const KeysaverConfig::Configuration& config, Mask* mask) const {
        if (!mask || !mask->empty()) return KeysaverStatus::E_INVALID_ARG;

        uint8_t rnd_byte = 0;
        uint32_t upper_count = 0;
        uint32_t lower_count = 0;
        uint32_t spec_count = 0;
        uint32_t digits_count = 0;

        auto letters_count = config.length();
        if (config.use_special_chars()) {
            spec_count = config.special_chars_count();
            letters_count -= spec_count;
        }
        if (config.use_digits()) {
            digits_count = config.digits_amount();
            letters_count -= digits_count;
        }

        if (config.use_upper() && config.use_lower()) {
            if (!m_prng.GetByte(&rnd_byte)) return KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

            lower_count = (rnd_byte % (letters_count - 1)) + 1;
            upper_count = letters_count - lower_count;
        }
        else if (config.use_upper()) {
            upper_count = letters_count;
        }
        else {
            lower_count = letters_count;
        }

        mask->insert(mask->end(), upper_count, SymbolType::UPPERCASE_LETTER);
        mask->insert(mask->end(), lower_count, SymbolType::LOWERCASE_LETTER);
        mask->insert(mask->end(), spec_count, SymbolType::SPECIAL_CHAR);
        mask->insert(mask->end(), digits_count, SymbolType::DIGIT);

        assert(mask->size() == config.length());
        std::ranges::shuffle(mask->begin(), mask->end(), m_prng);
        return KeysaverStatus::S_OK;
    }

    KeysaverStatus PasswordGenerator::ConstructPassword(
             const HashProvider::Hash& init_vector,
             const KeysaverConfig::Configuration& config,
             std::u8string* result) const {
        m_prng.ChangeIV(init_vector);

        Mask mask{};
        auto code = MakeMask(config, &mask);
        if (is_keysaver_error(code)) return code;

        return KeysaverStatus::E_NOT_IMPLEMENTED;
    }

}
