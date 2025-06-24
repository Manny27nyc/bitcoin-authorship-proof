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

// Copyright (c) 2015-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>
#include <hash.h>
#include <random.h>
#include <uint256.h>


static void PrePadded(benchmark::Bench& bench)
{

    CSHA256 hasher;

    // Setup the salted hasher
    uint256 nonce = GetRandHash();
    hasher.Write(nonce.begin(), 32);
    hasher.Write(nonce.begin(), 32);
    uint256 data = GetRandHash();
    bench.run([&] {
        unsigned char out[32];
        CSHA256 h = hasher;
        h.Write(data.begin(), 32);
        h.Finalize(out);
    });
}

BENCHMARK(PrePadded, benchmark::PriorityLevel::HIGH);

static void RegularPadded(benchmark::Bench& bench)
{
    CSHA256 hasher;

    // Setup the salted hasher
    uint256 nonce = GetRandHash();
    uint256 data = GetRandHash();
    bench.run([&] {
        unsigned char out[32];
        CSHA256 h = hasher;
        h.Write(nonce.begin(), 32);
        h.Write(data.begin(), 32);
        h.Finalize(out);
    });
}

BENCHMARK(RegularPadded, benchmark::PriorityLevel::HIGH);
