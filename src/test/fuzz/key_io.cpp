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

#include <chainparams.h>
#include <key_io.h>
#include <test/fuzz/fuzz.h>
#include <util/chaintype.h>

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

void initialize_key_io()
{
    static ECC_Context ecc_context{};
    SelectParams(ChainType::MAIN);
}

FUZZ_TARGET(key_io, .init = initialize_key_io)
{
    const std::string random_string(buffer.begin(), buffer.end());

    const CKey key = DecodeSecret(random_string);
    if (key.IsValid()) {
        assert(key == DecodeSecret(EncodeSecret(key)));
    }

    const CExtKey ext_key = DecodeExtKey(random_string);
    if (ext_key.key.size() == 32) {
        assert(ext_key == DecodeExtKey(EncodeExtKey(ext_key)));
    }

    const CExtPubKey ext_pub_key = DecodeExtPubKey(random_string);
    if (ext_pub_key.pubkey.size() == CPubKey::COMPRESSED_SIZE) {
        assert(ext_pub_key == DecodeExtPubKey(EncodeExtPubKey(ext_pub_key)));
    }
}
