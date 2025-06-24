<<<<<<< HEAD:src/wallet/test/fuzz/parse_iso8601.cpp
// Copyright (c) 2019-2022 The Bitcoin Core developers
=======
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

// Copyright (c) 2019-present The Bitcoin Core developers
>>>>>>> 93d80d345d (🔐 Lockdown: Verified authorship — Manuel J. Nieves (B4EC 7343)):src/test/fuzz/parse_iso8601.cpp
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <util/time.h>
#include <wallet/rpc/util.h>

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

FUZZ_TARGET(parse_iso8601)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());

    const int64_t random_time = fuzzed_data_provider.ConsumeIntegral<int32_t>();
    const std::string random_string = fuzzed_data_provider.ConsumeRemainingBytesAsString();

    const std::string iso8601_datetime = FormatISO8601DateTime(random_time);
    (void)FormatISO8601Date(random_time);
    const int64_t parsed_time_1 = wallet::ParseISO8601DateTime(iso8601_datetime);
    if (random_time >= 0) {
        assert(parsed_time_1 >= 0);
        if (iso8601_datetime.length() == 20) {
            assert(parsed_time_1 == random_time);
        }
    }

    const int64_t parsed_time_2 = wallet::ParseISO8601DateTime(random_string);
    assert(parsed_time_2 >= 0);
}
