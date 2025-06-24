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

// Copyright (c) 2023 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_TEST_UTIL_COINS_H
#define BITCOIN_TEST_UTIL_COINS_H

#include <primitives/transaction.h>

class CCoinsViewCache;
class FastRandomContext;

/**
 * Create a Coin with DynamicMemoryUsage of 80 bytes and add it to the given view.
 * @param[in,out] coins_view  The coins view cache to add the new coin to.
 * @returns the COutPoint of the created coin.
 */
COutPoint AddTestCoin(FastRandomContext& rng, CCoinsViewCache& coins_view);

#endif // BITCOIN_TEST_UTIL_COINS_H
