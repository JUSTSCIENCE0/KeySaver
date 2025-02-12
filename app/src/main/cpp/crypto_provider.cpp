// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "crypto_provider.hpp"

#include <openssl/evp.h>

#include <cassert>
#include <random>

namespace Keysaver {
    constexpr uint8_t salts[SALTS_COUNT][HashProvider::HASH_SIZE] = {
        { // cat
            0x52, 0x63, 0xd8, 0xb3, 0x80, 0xa6, 0x71, 0xed,
            0xe7, 0x8c, 0x37, 0x9a, 0xec, 0x2f, 0x3a, 0xb8,
            0x20, 0x10, 0xdc, 0xab, 0xe6, 0xf3, 0x0d, 0xbc,
            0x41, 0xc4, 0x90, 0xbd, 0x19, 0x6d, 0x1b, 0x31
        },
        { // car
            0x36, 0xfe, 0x75, 0xfd, 0x27, 0x86, 0xa3, 0x53,
            0x5c, 0xec, 0xed, 0x07, 0x44, 0xe0, 0x0b, 0xb7,
            0x59, 0xcc, 0x8b, 0xa3, 0xda, 0x07, 0x6f, 0x3c,
            0x8c, 0xed, 0x11, 0x4a, 0x2e, 0xd7, 0x04, 0xcb
        },
        { // egg
            0xa4, 0xf4, 0x78, 0xb9, 0xa3, 0xac, 0xa1, 0x6a,
            0xd1, 0x48, 0x15, 0x27, 0x06, 0x8f, 0xc0, 0xce,
            0x5f, 0xf2, 0x44, 0xc7, 0xa5, 0xc3, 0xf3, 0x8d,
            0x87, 0x26, 0x3a, 0x7e, 0xfc, 0xe4, 0xeb, 0x7e
        },
        { // city
            0xb7, 0x2c, 0x57, 0x3a, 0x17, 0x0a, 0x79, 0x57,
            0xce, 0x37, 0x38, 0xe6, 0x24, 0x17, 0x9c, 0x4b,
            0xc4, 0xe3, 0x64, 0xd4, 0xb0, 0xce, 0xf7, 0x3e,
            0x68, 0xcc, 0x7f, 0xb3, 0xc2, 0x3e, 0x4f, 0x3d
        },
        { // bot
            0x1f, 0x78, 0x1b, 0xb5, 0x4b, 0x0c, 0x3a, 0x96,
            0x25, 0x79, 0xed, 0xd3, 0x0e, 0xf1, 0x50, 0x3b,
            0xe0, 0xa8, 0x2d, 0x28, 0xda, 0xd5, 0x6c, 0xe8,
            0xb2, 0x9c, 0xe0, 0xd6, 0xef, 0x7a, 0xa7, 0x66
        },
        { // girl
            0xff, 0x23, 0x4e, 0x12, 0x6e, 0x1c, 0x69, 0x5c,
            0xef, 0x52, 0x95, 0x22, 0x32, 0xad, 0xec, 0xa4,
            0x7b, 0x36, 0xf0, 0x94, 0xa2, 0xb2, 0x21, 0x54,
            0x08, 0x47, 0x91, 0x22, 0x73, 0x5b, 0x47, 0x72
        },
        { // phone
            0x4e, 0xdc, 0x76, 0x4e, 0xea, 0xfd, 0x6d, 0x2f,
            0xf1, 0x99, 0x83, 0x46, 0xc8, 0x75, 0x62, 0x5d,
            0x78, 0xd8, 0x79, 0x51, 0x84, 0xed, 0x5d, 0x5f,
            0x20, 0x86, 0xa0, 0xc7, 0x48, 0xb7, 0x91, 0xf0
        },
        { // trees
            0x0f, 0x92, 0x8c, 0xbd, 0xf0, 0xdb, 0x34, 0xd0,
            0xa6, 0xe4, 0x29, 0xd5, 0xfb, 0x62, 0x3a, 0xb3,
            0x63, 0x77, 0x49, 0x26, 0x5a, 0x23, 0xd1, 0xa8,
            0x25, 0xb6, 0x68, 0x78, 0xc6, 0xf3, 0x2f, 0xfe
        },
        { // plane
            0x1a, 0x41, 0x27, 0xcd, 0xc1, 0x4f, 0x6c, 0x4f,
            0x4f, 0xab, 0x1b, 0xbe, 0x51, 0x75, 0xdd, 0xf5,
            0xb2, 0xea, 0xbb, 0xbc, 0x6c, 0x16, 0xa7, 0x33,
            0x60, 0x21, 0xc2, 0xbd, 0xe5, 0x66, 0x84, 0x22
        }
    };

    const EVP_MD* GetHashImpl(HashProvider::HashAlgorithm alg) {
        switch (alg) {
        case HashProvider::HashAlgorithm::SHA3_256:
            return EVP_sha3_256();
        case HashProvider::HashAlgorithm::BLAKE2_256:
            return EVP_blake2s256();
        default:
            assert(!"unexpected behavior");
            return nullptr;
        }
    }

    static inline PRNGProvider::PRNGIV HashToIV(const HashProvider::Hash& hash) {
        static_assert(HashProvider::HASH_SIZE == PRNGProvider::PRNG_IV_SIZE * 2);

        PRNGProvider::PRNGIV result{};

        for (size_t i = 0; i < PRNGProvider::PRNG_IV_SIZE; i++) {
            result[i] = hash[i] ^ hash[i + PRNGProvider::PRNG_IV_SIZE];
        }

        return result;
    }

    HashProvider::HashProvider() {
        m_ossl_ctx = EVP_MD_CTX_new();
        if (!m_ossl_ctx) throw KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;
    }

    HashProvider::~HashProvider() {
        EVP_MD_CTX_free(m_ossl_ctx);
        m_ossl_ctx = nullptr;
    }

    bool HashProvider::CalculateHash(
            const void* data, size_t size, HashAlgorithm alg, void* output, int saltID) {
        const EVP_MD* md = GetHashImpl(alg);
        if (!md) return false;

        if (EVP_DigestInit_ex(m_ossl_ctx, md, nullptr) != 1)
            return false;

        if (0 <= saltID && saltID < SALTS_COUNT) {
            if (EVP_DigestUpdate(m_ossl_ctx, salts[saltID], HASH_SIZE) != 1)
                return false;
        }

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

    PRNGProvider::PRNGProvider(const PRNGKey& key, const PRNGIV& iv) :
            m_key(key) {
        m_ossl_ctx = EVP_CIPHER_CTX_new();
        if (!m_ossl_ctx) throw KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

        if (EVP_EncryptInit_ex(
                m_ossl_ctx, EVP_aes_256_ofb(), nullptr,
                key.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(m_ossl_ctx);
            throw KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;
        }

        if (!UpdateBlock()) {
            EVP_CIPHER_CTX_free(m_ossl_ctx);
            throw KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;
        }
    }

    PRNGProvider::PRNGProvider(const PRNGKey& key) :
            PRNGProvider(key, PRNGIV{}) {}

    PRNGProvider::PRNGProvider(const PRNGKey& key, const HashProvider::Hash& iv) :
            PRNGProvider(key, HashToIV(iv)) {}

    PRNGProvider::~PRNGProvider() {
        EVP_CIPHER_CTX_free(m_ossl_ctx);
    }

    bool PRNGProvider::changeIV(const PRNGIV& iv) {
        int out_len = 0;
        if (EVP_EncryptFinal_ex(m_ossl_ctx, m_buffer.data(), &out_len) != 1)
            return false;

        if (EVP_EncryptInit_ex(
                m_ossl_ctx, EVP_aes_256_ofb(), nullptr,
                m_key.data(), iv.data()) != 1)
            return false;

        return UpdateBlock();
    }

    bool PRNGProvider::changeIV(const HashProvider::Hash& iv) {
        return changeIV(HashToIV(iv));
    }

    bool PRNGProvider::getByte(uint8_t* result) {
        *result = m_buffer[m_buffer_offset];
        m_buffer_offset++;

        if (m_buffer_offset >= BLOCK_SIZE)
            return UpdateBlock();

        return true;
    }

    bool PRNGProvider::getBytes(uint8_t* result, size_t count) {
        for (size_t i = 0; i < count; i++) {
            if (!getByte(result)) return false;
            result++;
        }

        return true;
    }

    PRNGProvider::result_type PRNGProvider::operator()() {
        result_type result = 0;
        uint8_t tmp_byte = 0;

        for (size_t i = 0; i < sizeof(result_type); i++) {
            if (!getByte(&tmp_byte))
                throw KeysaverStatus::E_INTERNAL_OPENSSL_FAIL;

            result <<= 8;
            result |= tmp_byte;
        }

        return result;
    }

    PRNGProvider::PRNGIV PRNGProvider::GenerateIV() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint8_t> dist(0, 255);

        PRNGIV result{};
        for (auto& byte : result) {
            byte = dist(gen);
        }
        return result;
    }

    bool PRNGProvider::UpdateBlock() {
        int out_len = 0;
        if (EVP_EncryptUpdate(m_ossl_ctx, m_buffer.data(), &out_len,
                              m_buffer.data(), BLOCK_SIZE) != 1)
            return false;

        assert(out_len == BLOCK_SIZE);
        m_buffer_offset = 0;
        return true;
    }
} // Keysaver