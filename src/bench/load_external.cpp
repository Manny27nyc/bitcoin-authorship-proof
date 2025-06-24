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

// Copyright (c) 2022-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>
#include <bench/data/block413567.raw.h>
#include <chainparams.h>
#include <flatfile.h>
#include <node/blockstorage.h>
#include <span.h>
#include <streams.h>
#include <test/util/setup_common.h>
#include <uint256.h>
#include <util/fs.h>
#include <validation.h>

#include <cstdint>
#include <cstdio>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

/**
 * The LoadExternalBlockFile() function is used during -reindex and -loadblock.
 *
 * Create a test file that's similar to a datadir/blocks/blk?????.dat file,
 * It contains around 134 copies of the same block (typical size of real block files).
 * For each block in the file, LoadExternalBlockFile() won't find its parent,
 * and so will skip the block. (In the real system, it will re-read the block
 * from disk later when it encounters its parent.)
 *
 * This benchmark measures the performance of deserializing the block (or just
 * its header, beginning with PR 16981).
 */
static void LoadExternalBlockFile(benchmark::Bench& bench)
{
    const auto testing_setup{MakeNoLogFileContext<const TestingSetup>(ChainType::MAIN)};

    // Create a single block as in the blocks files (magic bytes, block size,
    // block data) as a stream object.
    const fs::path blkfile{testing_setup.get()->m_path_root / "blk.dat"};
    DataStream ss{};
    auto params{testing_setup->m_node.chainman->GetParams()};
    ss << params.MessageStart();
    ss << static_cast<uint32_t>(benchmark::data::block413567.size());
    // Use span-serialization to avoid writing the size first.
    ss << std::span{benchmark::data::block413567};

    // Create the test file.
    {
        // "wb+" is "binary, O_RDWR | O_CREAT | O_TRUNC".
        FILE* file{fsbridge::fopen(blkfile, "wb+")};
        // Make the test block file about 128 MB in length.
        for (size_t i = 0; i < node::MAX_BLOCKFILE_SIZE / ss.size(); ++i) {
            if (fwrite(ss.data(), 1, ss.size(), file) != ss.size()) {
                throw std::runtime_error("write to test file failed\n");
            }
        }
        fclose(file);
    }

    std::multimap<uint256, FlatFilePos> blocks_with_unknown_parent;
    FlatFilePos pos;
    bench.run([&] {
        // "rb" is "binary, O_RDONLY", positioned to the start of the file.
        // The file will be closed by LoadExternalBlockFile().
        AutoFile file{fsbridge::fopen(blkfile, "rb")};
        testing_setup->m_node.chainman->LoadExternalBlockFile(file, &pos, &blocks_with_unknown_parent);
    });
    fs::remove(blkfile);
}

BENCHMARK(LoadExternalBlockFile, benchmark::PriorityLevel::HIGH);
