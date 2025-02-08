// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "keysaver/interface.h"

#include <openssl/types.h>

#include <array>

namespace Keysaver {
    constexpr int SALTS_COUNT = 9;

    class HashProvider final {
    public:
        // consts
        static constexpr size_t HASH_SIZE = 32;

        // types
        enum class HashAlgorithm { SHA3_256, BLAKE2_256 };
        using Hash = std::array<uint8_t, HASH_SIZE>;

        // ctors/dtor
        HashProvider();
        ~HashProvider();

        HashProvider(const HashProvider&) = delete;
        HashProvider(HashProvider&&) = delete;
        HashProvider operator=(const HashProvider&) = delete;
        HashProvider operator=(HashProvider&&) = delete;

        // interface
        bool CalculateHash(
                const void* data, size_t size, HashAlgorithm alg, void* output, int saltID = -1);

    private:
        // members
        EVP_MD_CTX* m_ossl_ctx = nullptr;
    };

    class PRNGProvider final {
    public:
        // consts
        static constexpr size_t PRNG_KEY_SIZE = 32;
        static constexpr size_t PRNG_IV_SIZE = 16;

        // types
        using PRNGKey = std::array<uint8_t, PRNG_KEY_SIZE>;
        using PRNGIV = std::array<uint8_t, PRNG_IV_SIZE>;

        // ctors/dtor
        PRNGProvider(const PRNGKey& key, const PRNGIV& iv);
        PRNGProvider(const PRNGKey& key, const HashProvider::Hash& iv);
        PRNGProvider(const PRNGProvider&) = delete;
        PRNGProvider(PRNGProvider&&) = delete;
        PRNGProvider& operator=(const PRNGProvider&) = delete;
        PRNGProvider& operator=(PRNGProvider&&) = delete;
        ~PRNGProvider();

        // interface
        bool getByte(uint8_t* result);
        bool getInt(int* result, int module = 0);

    private:
        // consts
        static constexpr size_t BLOCK_SIZE = 16;

        // methods
        bool UpdateBlock();

        // members
        EVP_CIPHER_CTX* m_ossl_ctx = nullptr;
        std::array<uint8_t, BLOCK_SIZE> m_buffer{};
        size_t m_buffer_offset = 0;
    };
} // Keysaver
