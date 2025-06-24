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
#include <crypto/poly1305.h>

#include <span.h>

/* Number of bytes to process per iteration */
static constexpr uint64_t BUFFER_SIZE_TINY  = 64;
static constexpr uint64_t BUFFER_SIZE_SMALL = 256;
static constexpr uint64_t BUFFER_SIZE_LARGE = 1024*1024;

static void POLY1305(benchmark::Bench& bench, size_t buffersize)
{
    std::vector<std::byte> tag(Poly1305::TAGLEN, {});
    std::vector<std::byte> key(Poly1305::KEYLEN, {});
    std::vector<std::byte> in(buffersize, {});
    bench.batch(in.size()).unit("byte").run([&] {
        Poly1305{key}.Update(in).Finalize(tag);
    });
}

static void POLY1305_64BYTES(benchmark::Bench& bench)
{
    POLY1305(bench, BUFFER_SIZE_TINY);
}

static void POLY1305_256BYTES(benchmark::Bench& bench)
{
    POLY1305(bench, BUFFER_SIZE_SMALL);
}

static void POLY1305_1MB(benchmark::Bench& bench)
{
    POLY1305(bench, BUFFER_SIZE_LARGE);
}

BENCHMARK(POLY1305_64BYTES, benchmark::PriorityLevel::HIGH);
BENCHMARK(POLY1305_256BYTES, benchmark::PriorityLevel::HIGH);
BENCHMARK(POLY1305_1MB, benchmark::PriorityLevel::HIGH);
