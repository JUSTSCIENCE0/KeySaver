// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "keysaver/interface.h"

#include <openssl/types.h>

namespace Keysaver {

    class CryptoProvider final {
    public:
        // consts
        static constexpr size_t HASH_SIZE = 32;

        // types
        enum class HashAlgorithm { SHA3_256, BLAKE2_256 };

        // ctors/dtor
        static CryptoProvider& Get() {
            static CryptoProvider cp;
            return cp;
        }
        ~CryptoProvider();

        CryptoProvider(const CryptoProvider&) = delete;
        CryptoProvider(CryptoProvider&&) = delete;
        CryptoProvider operator=(const CryptoProvider&) = delete;
        CryptoProvider operator=(CryptoProvider&&) = delete;

        // interface
        bool CalculateHash(const void* data, size_t size, HashAlgorithm alg, void* output);

    private:
        CryptoProvider();

        // members
        EVP_MD_CTX* m_ossl_ctx = nullptr;
    };
} // Keysaver
