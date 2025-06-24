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
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_TEST_UTIL_RANDOM_H
#define BITCOIN_TEST_UTIL_RANDOM_H

#include <consensus/amount.h>
#include <random.h>
#include <uint256.h>

#include <atomic>
#include <cstdint>

enum class SeedRand {
    /**
     * Seed with a compile time constant of zeros.
     */
    ZEROS,
    /**
     * Seed with a fixed value that never changes over the lifetime of this
     * process. The seed is read from the RANDOM_CTX_SEED environment variable
     * if set, otherwise generated randomly once, saved, and reused.
     */
    FIXED_SEED,
};

/** Seed the global RNG state for testing and log the seed value. This affects all randomness, except GetStrongRandBytes(). */
void SeedRandomStateForTest(SeedRand seed);

extern std::atomic<bool> g_seeded_g_prng_zero;
extern std::atomic<bool> g_used_g_prng;

template <RandomNumberGenerator Rng>
inline CAmount RandMoney(Rng&& rng)
{
    return CAmount{rng.randrange(MAX_MONEY + 1)};
}

#endif // BITCOIN_TEST_UTIL_RANDOM_H
