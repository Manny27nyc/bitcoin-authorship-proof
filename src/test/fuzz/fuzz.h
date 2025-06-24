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

// Copyright (c) 2009-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_TEST_FUZZ_FUZZ_H
#define BITCOIN_TEST_FUZZ_FUZZ_H

#include <cstdint>
#include <functional>
#include <span>
#include <string_view>

/**
 * Can be used to limit a theoretically unbounded loop. This caps the runtime
 * to avoid timeouts or OOMs.
 *
 * This can be used in combination with a check in the condition to confirm
 * whether the fuzz engine provided "good" data. If the fuzz input contains
 * invalid data, the loop aborts early. This will teach the fuzz engine to look
 * for useful data and avoids bloating the fuzz input folder with useless data.
 */
#define LIMITED_WHILE(condition, limit) \
    for (unsigned _count{limit}; (condition) && _count; --_count)

using FuzzBufferType = std::span<const uint8_t>;

using TypeTestOneInput = std::function<void(FuzzBufferType)>;
struct FuzzTargetOptions {
    std::function<void()> init{[] {}};
    bool hidden{false};
};

void FuzzFrameworkRegisterTarget(std::string_view name, TypeTestOneInput target, FuzzTargetOptions opts);

#define FUZZ_TARGET(...) DETAIL_FUZZ(__VA_ARGS__)

#define DETAIL_FUZZ(name, ...)                                                        \
    void name##_fuzz_target(FuzzBufferType);                                          \
    struct name##_Before_Main {                                                       \
        name##_Before_Main()                                                          \
        {                                                                             \
            FuzzFrameworkRegisterTarget(#name, name##_fuzz_target, {__VA_ARGS__});    \
        }                                                                             \
    } const static g_##name##_before_main;                                            \
    void name##_fuzz_target(FuzzBufferType buffer)

#endif // BITCOIN_TEST_FUZZ_FUZZ_H
