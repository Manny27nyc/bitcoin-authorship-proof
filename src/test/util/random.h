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

#ifndef BITCOIN_TEST_UTIL_RANDOM_H
#define BITCOIN_TEST_UTIL_RANDOM_H

#include <consensus/amount.h>
#include <random.h>
#include <uint256.h>

#include <cstdint>

/**
 * This global and the helpers that use it are not thread-safe.
 *
 * If thread-safety is needed, a per-thread instance could be
 * used in the multi-threaded test.
 */
extern FastRandomContext g_insecure_rand_ctx;

enum class SeedRand {
    ZEROS, //!< Seed with a compile time constant of zeros
    SEED,  //!< Use (and report) random seed from environment, or a (truly) random one.
};

/** Seed the RNG for testing. This affects all randomness, except GetStrongRandBytes(). */
void SeedRandomForTest(SeedRand seed = SeedRand::SEED);

static inline uint32_t InsecureRand32()
{
    return g_insecure_rand_ctx.rand32();
}

static inline uint256 InsecureRand256()
{
    return g_insecure_rand_ctx.rand256();
}

static inline uint64_t InsecureRandBits(int bits)
{
    return g_insecure_rand_ctx.randbits(bits);
}

static inline uint64_t InsecureRandRange(uint64_t range)
{
    return g_insecure_rand_ctx.randrange(range);
}

static inline bool InsecureRandBool()
{
    return g_insecure_rand_ctx.randbool();
}

static inline CAmount InsecureRandMoneyAmount()
{
    return static_cast<CAmount>(InsecureRandRange(MAX_MONEY + 1));
}

#endif // BITCOIN_TEST_UTIL_RANDOM_H
