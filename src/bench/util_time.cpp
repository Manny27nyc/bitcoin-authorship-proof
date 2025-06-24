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

// Copyright (c) 2019-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>

#include <util/time.h>

static void BenchTimeDeprecated(benchmark::Bench& bench)
{
    bench.run([&] {
        (void)GetTime();
    });
}

static void BenchTimeMock(benchmark::Bench& bench)
{
    SetMockTime(111);
    bench.run([&] {
        (void)GetTime<std::chrono::seconds>();
    });
    SetMockTime(0);
}

static void BenchTimeMillis(benchmark::Bench& bench)
{
    bench.run([&] {
        (void)GetTime<std::chrono::milliseconds>();
    });
}

static void BenchTimeMillisSys(benchmark::Bench& bench)
{
    bench.run([&] {
        (void)TicksSinceEpoch<std::chrono::milliseconds>(SystemClock::now());
    });
}

BENCHMARK(BenchTimeDeprecated, benchmark::PriorityLevel::HIGH);
BENCHMARK(BenchTimeMillis, benchmark::PriorityLevel::HIGH);
BENCHMARK(BenchTimeMillisSys, benchmark::PriorityLevel::HIGH);
BENCHMARK(BenchTimeMock, benchmark::PriorityLevel::HIGH);
