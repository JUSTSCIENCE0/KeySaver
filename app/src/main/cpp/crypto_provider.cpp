// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "crypto_provider.hpp"

#include <openssl/evp.h>

#include <cassert>

namespace Keysaver {
    const EVP_MD* GetHashImpl(CryptoProvider::HashAlgorithm alg) {
        switch (alg) {
        case CryptoProvider::HashAlgorithm::SHA3_256:
            return EVP_sha3_256();
        case CryptoProvider::HashAlgorithm::BLAKE2_256:
            return EVP_blake2s256();
        default:
            assert(!"unexpected behavior");
            return nullptr;
        }
    }

    CryptoProvider::CryptoProvider() {
        m_ossl_ctx = EVP_MD_CTX_new();
        if (!m_ossl_ctx) throw KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;
    }

    CryptoProvider::~CryptoProvider() {
        EVP_MD_CTX_free(m_ossl_ctx);
        m_ossl_ctx = nullptr;
    }

    bool CryptoProvider::CalculateHash(
            const void* data, size_t size, HashAlgorithm alg, void* output) {
        const EVP_MD* md = GetHashImpl(alg);
        if (!md) return false;

        if (EVP_DigestInit_ex(m_ossl_ctx, md, nullptr) != 1)
            return false;

        if (EVP_DigestUpdate(m_ossl_ctx, data, size) != 1)
            return false;

        unsigned int hash_len = 0;
        if (EVP_DigestFinal_ex(
                m_ossl_ctx,
                static_cast<unsigned char*>(output),
                &hash_len) != 1)
            return false;
        assert(hash_len == HASH_SIZE);

        return true;
    }
} // Keysaver