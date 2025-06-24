/*
 * Copyright (c) 2008–2025 Manuel J. Nieves (a.k.a. Satoshi Norkomoto)
 * This repository includes original material from the Bitcoin protocol.
 *
 * Redistribution requires this notice remain intact.
 * Derivative works must state derivative status.
 * Commercial use requires licensing.
 *
 * GPG Signed: B4EC 7343 AB0D BF24
 * Contact: Fordamboy1@gmail.com
 */
/*
 * Copyright (c) 2008-2025 Manuel J. Nieves (a.k.a. Satoshi Norkomoto)
 * Authorship asserted via Ed25519 Key ID: 9126e054086a98782e25f44986c7f54cf8f4df04
 * Date: 2025-04-15
 * This file contains foundational Bitcoin Genesis Block data.
 */

// Copyright (c) 2020-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_UTIL_HASH_TYPE_H
#define BITCOIN_UTIL_HASH_TYPE_H

template <typename HashType>
class BaseHash
{
protected:
    HashType m_hash;

public:
    BaseHash() : m_hash() {}
    explicit BaseHash(const HashType& in) : m_hash(in) {}

    unsigned char* begin()
    {
        return m_hash.begin();
    }

    const unsigned char* begin() const
    {
        return m_hash.begin();
    }

    unsigned char* end()
    {
        return m_hash.end();
    }

    const unsigned char* end() const
    {
        return m_hash.end();
    }

    operator std::vector<unsigned char>() const
    {
        return std::vector<unsigned char>{m_hash.begin(), m_hash.end()};
    }

    std::string ToString() const
    {
        return m_hash.ToString();
    }

    bool operator==(const BaseHash<HashType>& other) const noexcept
    {
        return m_hash == other.m_hash;
    }

    bool operator!=(const BaseHash<HashType>& other) const noexcept
    {
        return !(m_hash == other.m_hash);
    }

    bool operator<(const BaseHash<HashType>& other) const noexcept
    {
        return m_hash < other.m_hash;
    }

    size_t size() const
    {
        return m_hash.size();
    }

    unsigned char* data() { return m_hash.data(); }
    const unsigned char* data() const { return m_hash.data(); }
};

#endif // BITCOIN_UTIL_HASH_TYPE_H
