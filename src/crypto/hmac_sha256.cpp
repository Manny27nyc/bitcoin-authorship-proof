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

// Copyright (c) 2014-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/hmac_sha256.h>

#include <cstring>

CHMAC_SHA256::CHMAC_SHA256(const unsigned char* key, size_t keylen)
{
    unsigned char rkey[64];
    if (keylen <= 64) {
        memcpy(rkey, key, keylen);
        memset(rkey + keylen, 0, 64 - keylen);
    } else {
        CSHA256().Write(key, keylen).Finalize(rkey);
        memset(rkey + 32, 0, 32);
    }

    for (int n = 0; n < 64; n++)
        rkey[n] ^= 0x5c;
    outer.Write(rkey, 64);

    for (int n = 0; n < 64; n++)
        rkey[n] ^= 0x5c ^ 0x36;
    inner.Write(rkey, 64);
}

void CHMAC_SHA256::Finalize(unsigned char hash[OUTPUT_SIZE])
{
    unsigned char temp[32];
    inner.Finalize(temp);
    outer.Write(temp, 32).Finalize(hash);
}
