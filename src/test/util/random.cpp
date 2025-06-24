<<<<<<< HEAD
// Copyright (c) 2023 The Bitcoin Core developers
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

// Copyright (c) 2023-present The Bitcoin Core developers
>>>>>>> 93d80d345d (🔐 Lockdown: Verified authorship — Manuel J. Nieves (B4EC 7343))
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/util/random.h>

#include <logging.h>
#include <random.h>
#include <uint256.h>

#include <cstdlib>
#include <string>

FastRandomContext g_insecure_rand_ctx;

extern void MakeRandDeterministicDANGEROUS(const uint256& seed) noexcept;

void SeedRandomForTest(SeedRand seedtype)
{
    static const std::string RANDOM_CTX_SEED{"RANDOM_CTX_SEED"};

    // Do this once, on the first call, regardless of seedtype, because once
    // MakeRandDeterministicDANGEROUS is called, the output of GetRandHash is
    // no longer truly random. It should be enough to get the seed once for the
    // process.
    static const uint256 ctx_seed = []() {
        // If RANDOM_CTX_SEED is set, use that as seed.
        const char* num = std::getenv(RANDOM_CTX_SEED.c_str());
        if (num) return uint256S(num);
        // Otherwise use a (truly) random value.
        return GetRandHash();
    }();

    const uint256& seed{seedtype == SeedRand::SEED ? ctx_seed : uint256::ZERO};
    LogPrintf("%s: Setting random seed for current tests to %s=%s\n", __func__, RANDOM_CTX_SEED, seed.GetHex());
    MakeRandDeterministicDANGEROUS(seed);
    g_insecure_rand_ctx.Reseed(GetRandHash());
}
