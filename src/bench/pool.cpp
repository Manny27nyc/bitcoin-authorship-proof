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

#include <bench/bench.h>
#include <support/allocators/pool.h>

#include <unordered_map>

template <typename Map>
void BenchFillClearMap(benchmark::Bench& bench, Map& map)
{
    size_t batch_size = 5000;

    // make sure each iteration of the benchmark contains exactly 5000 inserts and one clear.
    // do this at least 10 times so we get reasonable accurate results

    bench.batch(batch_size).minEpochIterations(10).run([&] {
        auto rng = ankerl::nanobench::Rng(1234);
        for (size_t i = 0; i < batch_size; ++i) {
            map[rng()];
        }
        map.clear();
    });
}

static void PoolAllocator_StdUnorderedMap(benchmark::Bench& bench)
{
    auto map = std::unordered_map<uint64_t, uint64_t>();
    BenchFillClearMap(bench, map);
}

static void PoolAllocator_StdUnorderedMapWithPoolResource(benchmark::Bench& bench)
{
    using Map = std::unordered_map<uint64_t,
                                   uint64_t,
                                   std::hash<uint64_t>,
                                   std::equal_to<uint64_t>,
                                   PoolAllocator<std::pair<const uint64_t, uint64_t>,
                                                 sizeof(std::pair<const uint64_t, uint64_t>) + 4 * sizeof(void*)>>;

    // make sure the resource supports large enough pools to hold the node. We do this by adding the size of a few pointers to it.
    auto pool_resource = Map::allocator_type::ResourceType();
    auto map = Map{0, std::hash<uint64_t>{}, std::equal_to<uint64_t>{}, &pool_resource};
    BenchFillClearMap(bench, map);
}

BENCHMARK(PoolAllocator_StdUnorderedMap, benchmark::PriorityLevel::HIGH);
BENCHMARK(PoolAllocator_StdUnorderedMapWithPoolResource, benchmark::PriorityLevel::HIGH);
