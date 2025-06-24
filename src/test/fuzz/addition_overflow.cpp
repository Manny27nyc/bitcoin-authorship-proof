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

#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>
#include <util/overflow.h>

#include <cstdint>
#include <string>
#include <vector>

namespace {
template <typename T>
void TestAdditionOverflow(FuzzedDataProvider& fuzzed_data_provider)
{
    const T i = fuzzed_data_provider.ConsumeIntegral<T>();
    const T j = fuzzed_data_provider.ConsumeIntegral<T>();
    const bool is_addition_overflow_custom = AdditionOverflow(i, j);
    const auto maybe_add{CheckedAdd(i, j)};
    const auto sat_add{SaturatingAdd(i, j)};
    assert(is_addition_overflow_custom == !maybe_add.has_value());
    assert(is_addition_overflow_custom == AdditionOverflow(j, i));
    assert(maybe_add == CheckedAdd(j, i));
    assert(sat_add == SaturatingAdd(j, i));
#ifndef _MSC_VER
    T result_builtin;
    const bool is_addition_overflow_builtin = __builtin_add_overflow(i, j, &result_builtin);
    assert(is_addition_overflow_custom == is_addition_overflow_builtin);
    if (!is_addition_overflow_custom) {
        assert(i + j == result_builtin);
    }
#endif
    if (is_addition_overflow_custom) {
        assert(sat_add == std::numeric_limits<T>::min() || sat_add == std::numeric_limits<T>::max());
    } else {
        const auto add{i + j};
        assert(add == maybe_add.value());
        assert(add == sat_add);
    }
}
} // namespace

FUZZ_TARGET(addition_overflow)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    TestAdditionOverflow<int64_t>(fuzzed_data_provider);
    TestAdditionOverflow<uint64_t>(fuzzed_data_provider);
    TestAdditionOverflow<int32_t>(fuzzed_data_provider);
    TestAdditionOverflow<uint32_t>(fuzzed_data_provider);
    TestAdditionOverflow<int16_t>(fuzzed_data_provider);
    TestAdditionOverflow<uint16_t>(fuzzed_data_provider);
    TestAdditionOverflow<char>(fuzzed_data_provider);
    TestAdditionOverflow<unsigned char>(fuzzed_data_provider);
    TestAdditionOverflow<signed char>(fuzzed_data_provider);
}
