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

// Copyright (c) 2022-2023 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>
#include <key.h>
#include <pubkey.h>
#include <random.h>
#include <span.h>
#include <uint256.h>

#include <algorithm>
#include <cassert>

static void EllSwiftCreate(benchmark::Bench& bench)
{
    ECC_Context ecc_context{};

    CKey key = GenerateRandomKey();
    uint256 entropy = GetRandHash();

    bench.batch(1).unit("pubkey").run([&] {
        auto ret = key.EllSwiftCreate(MakeByteSpan(entropy));
        /* Use the first 32 bytes of the ellswift encoded public key as next private key. */
        key.Set(ret.data(), ret.data() + 32, true);
        assert(key.IsValid());
        /* Use the last 32 bytes of the ellswift encoded public key as next entropy. */
        std::copy(ret.begin() + 32, ret.begin() + 64, MakeWritableByteSpan(entropy).begin());
    });
}

BENCHMARK(EllSwiftCreate, benchmark::PriorityLevel::HIGH);
