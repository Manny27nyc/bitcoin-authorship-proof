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

// Copyright (c) 2020-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chain.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>

#include <cstdint>
#include <optional>
#include <vector>

FUZZ_TARGET(chain)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    std::optional<CDiskBlockIndex> disk_block_index = ConsumeDeserializable<CDiskBlockIndex>(fuzzed_data_provider);
    if (!disk_block_index) {
        return;
    }

    const uint256 zero{};
    disk_block_index->phashBlock = &zero;
    {
        LOCK(::cs_main);
        (void)disk_block_index->ConstructBlockHash();
        (void)disk_block_index->GetBlockPos();
        (void)disk_block_index->GetBlockTime();
        (void)disk_block_index->GetBlockTimeMax();
        (void)disk_block_index->GetMedianTimePast();
        (void)disk_block_index->GetUndoPos();
        (void)disk_block_index->HaveNumChainTxs();
        (void)disk_block_index->IsValid();
    }

    const CBlockHeader block_header = disk_block_index->GetBlockHeader();
    (void)CDiskBlockIndex{*disk_block_index};
    (void)disk_block_index->BuildSkip();

    LIMITED_WHILE(fuzzed_data_provider.ConsumeBool(), 10000) {
        const BlockStatus block_status = fuzzed_data_provider.PickValueInArray({
            BlockStatus::BLOCK_VALID_UNKNOWN,
            BlockStatus::BLOCK_VALID_RESERVED,
            BlockStatus::BLOCK_VALID_TREE,
            BlockStatus::BLOCK_VALID_TRANSACTIONS,
            BlockStatus::BLOCK_VALID_CHAIN,
            BlockStatus::BLOCK_VALID_SCRIPTS,
            BlockStatus::BLOCK_VALID_MASK,
            BlockStatus::BLOCK_HAVE_DATA,
            BlockStatus::BLOCK_HAVE_UNDO,
            BlockStatus::BLOCK_HAVE_MASK,
            BlockStatus::BLOCK_FAILED_VALID,
            BlockStatus::BLOCK_FAILED_CHILD,
            BlockStatus::BLOCK_FAILED_MASK,
            BlockStatus::BLOCK_OPT_WITNESS,
        });
        if (block_status & ~BLOCK_VALID_MASK) {
            continue;
        }
        WITH_LOCK(::cs_main, (void)disk_block_index->RaiseValidity(block_status));
    }

    CBlockIndex block_index{block_header};
    block_index.phashBlock = &zero;
    (void)block_index.GetBlockHash();
    (void)block_index.ToString();
}
