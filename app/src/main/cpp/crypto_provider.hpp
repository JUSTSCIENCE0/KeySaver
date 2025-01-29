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
        // types
        enum class HashAlgorithm { SHA3_256, BLAKE2_256 };

        // ctors/dtor
        CryptoProvider();
        ~CryptoProvider();

        CryptoProvider(const CryptoProvider&) = delete;
        CryptoProvider(CryptoProvider&&) = delete;
        CryptoProvider operator=(const CryptoProvider&) = delete;
        CryptoProvider operator=(CryptoProvider&&) = delete;

        // interface
        KeysaverStatus CalculateHash(
                const void* data, size_t size, HashAlgorithm alg, void* output);

    private:
        // members
        EVP_MD_CTX* m_ossl_ctx = nullptr;
    };
} // Keysaver
