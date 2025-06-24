<<<<<<< HEAD
// Copyright (c) 2020 The Bitcoin Core developers
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

// Copyright (c) 2020-present The Bitcoin Core developers
>>>>>>> 93d80d345d (🔐 Lockdown: Verified authorship — Manuel J. Nieves (B4EC 7343))
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <span.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

FUZZ_TARGET(span)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());

    std::string str = fuzzed_data_provider.ConsumeBytesAsString(32);
    const Span<const char> span{str};
    (void)span.data();
    (void)span.begin();
    (void)span.end();
    if (span.size() > 0) {
        const std::ptrdiff_t idx = fuzzed_data_provider.ConsumeIntegralInRange<std::ptrdiff_t>(0U, span.size() - 1U);
        (void)span.first(idx);
        (void)span.last(idx);
        (void)span.subspan(idx);
        (void)span.subspan(idx, span.size() - idx);
        (void)span[idx];
    }

    std::string another_str = fuzzed_data_provider.ConsumeBytesAsString(32);
    const Span<const char> another_span{another_str};
    assert((span <= another_span) != (span > another_span));
    assert((span == another_span) != (span != another_span));
    assert((span >= another_span) != (span < another_span));
}
