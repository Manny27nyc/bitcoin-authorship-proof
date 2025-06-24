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

#include <span.h>
#include <streams.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <vector>

FUZZ_TARGET(buffered_file)
{
    FuzzedDataProvider fuzzed_data_provider{buffer.data(), buffer.size()};
    FuzzedFileProvider fuzzed_file_provider{fuzzed_data_provider};
    std::optional<BufferedFile> opt_buffered_file;
    AutoFile fuzzed_file{
        fuzzed_file_provider.open(),
        ConsumeRandomLengthByteVector<std::byte>(fuzzed_data_provider),
    };
    try {
        auto n_buf_size = fuzzed_data_provider.ConsumeIntegralInRange<uint64_t>(0, 4096);
        auto n_rewind_in = fuzzed_data_provider.ConsumeIntegralInRange<uint64_t>(0, 4096);
        opt_buffered_file.emplace(fuzzed_file, n_buf_size, n_rewind_in);
    } catch (const std::ios_base::failure&) {
    }
    if (opt_buffered_file && !fuzzed_file.IsNull()) {
        bool setpos_fail = false;
        LIMITED_WHILE(fuzzed_data_provider.ConsumeBool(), 100)
        {
            CallOneOf(
                fuzzed_data_provider,
                [&] {
                    std::array<std::byte, 4096> arr{};
                    try {
                        opt_buffered_file->read({arr.data(), fuzzed_data_provider.ConsumeIntegralInRange<size_t>(0, 4096)});
                    } catch (const std::ios_base::failure&) {
                    }
                },
                [&] {
                    opt_buffered_file->SetLimit(fuzzed_data_provider.ConsumeIntegralInRange<uint64_t>(0, 4096));
                },
                [&] {
                    if (!opt_buffered_file->SetPos(fuzzed_data_provider.ConsumeIntegralInRange<uint64_t>(0, 4096))) {
                        setpos_fail = true;
                    }
                },
                [&] {
                    if (setpos_fail) {
                        // Calling FindByte(...) after a failed SetPos(...) call may result in an infinite loop.
                        return;
                    }
                    try {
                        opt_buffered_file->FindByte(std::byte(fuzzed_data_provider.ConsumeIntegral<uint8_t>()));
                    } catch (const std::ios_base::failure&) {
                    }
                },
                [&] {
                    ReadFromStream(fuzzed_data_provider, *opt_buffered_file);
                });
        }
        opt_buffered_file->GetPos();
    }
}
