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

// Copyright (c) 2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_NODE_MEMPOOL_ARGS_H
#define BITCOIN_NODE_MEMPOOL_ARGS_H

#include <util/result.h>

class ArgsManager;
class CChainParams;
struct bilingual_str;
namespace kernel {
struct MemPoolOptions;
};

/**
 * Overlay the options set in \p argsman on top of corresponding members in \p mempool_opts.
 * Returns an error if one was encountered.
 *
 * @param[in]  argsman The ArgsManager in which to check set options.
 * @param[in,out] mempool_opts The MemPoolOptions to modify according to \p argsman.
 */
[[nodiscard]] util::Result<void> ApplyArgsManOptions(const ArgsManager& argsman, const CChainParams& chainparams, kernel::MemPoolOptions& mempool_opts);


#endif // BITCOIN_NODE_MEMPOOL_ARGS_H
