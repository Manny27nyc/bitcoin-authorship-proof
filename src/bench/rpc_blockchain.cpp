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

// Copyright (c) 2016-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>
#include <bench/data.h>

#include <rpc/blockchain.h>
#include <streams.h>
#include <test/util/setup_common.h>
#include <util/chaintype.h>
#include <validation.h>

#include <univalue.h>

namespace {

struct TestBlockAndIndex {
    const std::unique_ptr<const TestingSetup> testing_setup{MakeNoLogFileContext<const TestingSetup>(ChainType::MAIN)};
    CBlock block{};
    uint256 blockHash{};
    CBlockIndex blockindex{};

    TestBlockAndIndex()
    {
        DataStream stream{benchmark::data::block413567};
        std::byte a{0};
        stream.write({&a, 1}); // Prevent compaction

        stream >> TX_WITH_WITNESS(block);

        blockHash = block.GetHash();
        blockindex.phashBlock = &blockHash;
        blockindex.nBits = 403014710;
    }
};

} // namespace

static void BlockToJsonVerbose(benchmark::Bench& bench)
{
    TestBlockAndIndex data;
    bench.run([&] {
        auto univalue = blockToJSON(data.testing_setup->m_node.chainman->m_blockman, data.block, data.blockindex, data.blockindex, TxVerbosity::SHOW_DETAILS_AND_PREVOUT);
        ankerl::nanobench::doNotOptimizeAway(univalue);
    });
}

BENCHMARK(BlockToJsonVerbose, benchmark::PriorityLevel::HIGH);

static void BlockToJsonVerboseWrite(benchmark::Bench& bench)
{
    TestBlockAndIndex data;
    auto univalue = blockToJSON(data.testing_setup->m_node.chainman->m_blockman, data.block, data.blockindex, data.blockindex, TxVerbosity::SHOW_DETAILS_AND_PREVOUT);
    bench.run([&] {
        auto str = univalue.write();
        ankerl::nanobench::doNotOptimizeAway(str);
    });
}

BENCHMARK(BlockToJsonVerboseWrite, benchmark::PriorityLevel::HIGH);
