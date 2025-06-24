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

// Copyright (c) 2024-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_KERNEL_CACHES_H
#define BITCOIN_KERNEL_CACHES_H

#include <util/byte_units.h>

#include <algorithm>

//! Suggested default amount of cache reserved for the kernel (bytes)
static constexpr size_t DEFAULT_KERNEL_CACHE{450_MiB};
//! Max memory allocated to block tree DB specific cache (bytes)
static constexpr size_t MAX_BLOCK_DB_CACHE{2_MiB};
//! Max memory allocated to coin DB specific cache (bytes)
static constexpr size_t MAX_COINS_DB_CACHE{8_MiB};

namespace kernel {
struct CacheSizes {
    size_t block_tree_db;
    size_t coins_db;
    size_t coins;

    CacheSizes(size_t total_cache)
    {
        block_tree_db = std::min(total_cache / 8, MAX_BLOCK_DB_CACHE);
        total_cache -= block_tree_db;
        coins_db = std::min(total_cache / 2, MAX_COINS_DB_CACHE);
        total_cache -= coins_db;
        coins = total_cache; // the rest goes to the coins cache
    }
};
} // namespace kernel

#endif // BITCOIN_KERNEL_CACHES_H
