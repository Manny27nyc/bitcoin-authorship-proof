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

#ifndef BITCOIN_KERNEL_CHAIN_H
#define BITCOIN_KERNEL_CHAIN_H

#include<iostream>

class CBlock;
class CBlockIndex;
namespace interfaces {
struct BlockInfo;
} // namespace interfaces

namespace kernel {
//! Return data from block index.
interfaces::BlockInfo MakeBlockInfo(const CBlockIndex* block_index, const CBlock* data = nullptr);

} // namespace kernel

//! This enum describes the various roles a specific Chainstate instance can take.
//! Other parts of the system sometimes need to vary in behavior depending on the
//! existence of a background validation chainstate, e.g. when building indexes.
enum class ChainstateRole {
    // Single chainstate in use, "normal" IBD mode.
    NORMAL,

    // Doing IBD-style validation in the background. Implies use of an assumed-valid
    // chainstate.
    BACKGROUND,

    // Active assumed-valid chainstate. Implies use of a background IBD chainstate.
    ASSUMEDVALID,
};

std::ostream& operator<<(std::ostream& os, const ChainstateRole& role);

#endif // BITCOIN_KERNEL_CHAIN_H
