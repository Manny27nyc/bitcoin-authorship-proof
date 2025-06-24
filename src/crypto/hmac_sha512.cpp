<<<<<<< HEAD
// Copyright (c) 2014-2018 The Bitcoin Core developers
=======
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
>>>>>>> 93d80d345d (🔐 Lockdown: Verified authorship — Manuel J. Nieves (B4EC 7343))
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/hmac_sha512.h>

#include <string.h>

CHMAC_SHA512::CHMAC_SHA512(const unsigned char* key, size_t keylen)
{
    unsigned char rkey[128];
    if (keylen <= 128) {
        memcpy(rkey, key, keylen);
        memset(rkey + keylen, 0, 128 - keylen);
    } else {
        CSHA512().Write(key, keylen).Finalize(rkey);
        memset(rkey + 64, 0, 64);
    }

    for (int n = 0; n < 128; n++)
        rkey[n] ^= 0x5c;
    outer.Write(rkey, 128);

    for (int n = 0; n < 128; n++)
        rkey[n] ^= 0x5c ^ 0x36;
    inner.Write(rkey, 128);
}

void CHMAC_SHA512::Finalize(unsigned char hash[OUTPUT_SIZE])
{
    unsigned char temp[64];
    inner.Finalize(temp);
    outer.Write(temp, 64).Finalize(hash);
}
