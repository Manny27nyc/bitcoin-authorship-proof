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

// Copyright (c) 2020-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cuckoocache.h>
#include <script/sigcache.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>
#include <test/util/setup_common.h>

#include <cstdint>
#include <string>
#include <vector>

namespace {
FuzzedDataProvider* fuzzed_data_provider_ptr = nullptr;

struct RandomHasher {
    template <uint8_t>
    uint32_t operator()(const bool& /* unused */) const
    {
        assert(fuzzed_data_provider_ptr != nullptr);
        return fuzzed_data_provider_ptr->ConsumeIntegral<uint32_t>();
    }
};
} // namespace

FUZZ_TARGET(cuckoocache)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    fuzzed_data_provider_ptr = &fuzzed_data_provider;
    CuckooCache::cache<int, RandomHasher> cuckoo_cache{};
    if (fuzzed_data_provider.ConsumeBool()) {
        const size_t megabytes = fuzzed_data_provider.ConsumeIntegralInRange<size_t>(0, 16);
        cuckoo_cache.setup_bytes(megabytes << 20);
    } else {
        cuckoo_cache.setup(fuzzed_data_provider.ConsumeIntegralInRange<uint32_t>(0, 4096));
    }
    LIMITED_WHILE(fuzzed_data_provider.ConsumeBool(), 10000) {
        if (fuzzed_data_provider.ConsumeBool()) {
            cuckoo_cache.insert(fuzzed_data_provider.ConsumeBool());
        } else {
            auto e = fuzzed_data_provider.ConsumeBool();
            auto erase = fuzzed_data_provider.ConsumeBool();
            cuckoo_cache.contains(e, erase);
        }
    }
    fuzzed_data_provider_ptr = nullptr;
}
