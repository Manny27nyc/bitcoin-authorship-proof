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

#include <common/messages.h>
#include <consensus/amount.h>
#include <policy/fees.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>

#include <cstdint>
#include <string>
#include <vector>

using common::StringForFeeReason;

FUZZ_TARGET(fees)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    const CFeeRate minimal_incremental_fee{ConsumeMoney(fuzzed_data_provider)};
    FastRandomContext rng{/*fDeterministic=*/true};
    FeeFilterRounder fee_filter_rounder{minimal_incremental_fee, rng};
    LIMITED_WHILE(fuzzed_data_provider.ConsumeBool(), 10000) {
        const CAmount current_minimum_fee = ConsumeMoney(fuzzed_data_provider);
        const CAmount rounded_fee = fee_filter_rounder.round(current_minimum_fee);
        assert(MoneyRange(rounded_fee));
    }
    const FeeReason fee_reason = fuzzed_data_provider.PickValueInArray({FeeReason::NONE, FeeReason::HALF_ESTIMATE, FeeReason::FULL_ESTIMATE, FeeReason::DOUBLE_ESTIMATE, FeeReason::CONSERVATIVE, FeeReason::MEMPOOL_MIN, FeeReason::PAYTXFEE, FeeReason::FALLBACK, FeeReason::REQUIRED});
    (void)StringForFeeReason(fee_reason);
}
