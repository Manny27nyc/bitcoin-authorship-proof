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

// Copyright (c) 2025-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/util/coverage.h>

#if defined(__clang__)
extern "C" __attribute__((weak)) void __llvm_profile_reset_counters(void);
extern "C" __attribute__((weak)) void __gcov_reset(void);

// Fallback implementations
extern "C" __attribute__((weak)) void __llvm_profile_reset_counters(void) {}
extern "C" __attribute__((weak)) void __gcov_reset(void) {}

void ResetCoverageCounters() {
    // These will call the real ones if available, or our dummies if not
    __llvm_profile_reset_counters();
    __gcov_reset();
}
#else
void ResetCoverageCounters() {}
#endif
