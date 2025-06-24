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

#include <blockfilter.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

FUZZ_TARGET(blockfilter)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    const std::optional<BlockFilter> block_filter = ConsumeDeserializable<BlockFilter>(fuzzed_data_provider);
    if (!block_filter) {
        return;
    }
    {
        (void)block_filter->ComputeHeader(ConsumeUInt256(fuzzed_data_provider));
        (void)block_filter->GetBlockHash();
        (void)block_filter->GetEncodedFilter();
        (void)block_filter->GetHash();
    }
    {
        const BlockFilterType block_filter_type = block_filter->GetFilterType();
        (void)BlockFilterTypeName(block_filter_type);
    }
    {
        const GCSFilter gcs_filter = block_filter->GetFilter();
        (void)gcs_filter.GetN();
        (void)gcs_filter.GetParams();
        (void)gcs_filter.GetEncoded();
        (void)gcs_filter.Match(ConsumeRandomLengthByteVector(fuzzed_data_provider));
        GCSFilter::ElementSet element_set;
        LIMITED_WHILE(fuzzed_data_provider.ConsumeBool(), 30000)
        {
            element_set.insert(ConsumeRandomLengthByteVector(fuzzed_data_provider));
        }
        gcs_filter.MatchAny(element_set);
    }
}
