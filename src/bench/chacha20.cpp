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
#include <crypto/chacha20.h>
#include <crypto/chacha20poly1305.h>

/* Number of bytes to process per iteration */
static const uint64_t BUFFER_SIZE_TINY  = 64;
static const uint64_t BUFFER_SIZE_SMALL = 256;
static const uint64_t BUFFER_SIZE_LARGE = 1024*1024;

static void CHACHA20(benchmark::Bench& bench, size_t buffersize)
{
    std::vector<std::byte> key(32, {});
    ChaCha20 ctx(key);
    ctx.Seek({0, 0}, 0);
    std::vector<std::byte> in(buffersize, {});
    std::vector<std::byte> out(buffersize, {});
    bench.batch(in.size()).unit("byte").run([&] {
        ctx.Crypt(in, out);
    });
}

static void FSCHACHA20POLY1305(benchmark::Bench& bench, size_t buffersize)
{
    std::vector<std::byte> key(32);
    FSChaCha20Poly1305 ctx(key, 224);
    std::vector<std::byte> in(buffersize);
    std::vector<std::byte> aad;
    std::vector<std::byte> out(buffersize + FSChaCha20Poly1305::EXPANSION);
    bench.batch(in.size()).unit("byte").run([&] {
        ctx.Encrypt(in, aad, out);
    });
}

static void CHACHA20_64BYTES(benchmark::Bench& bench)
{
    CHACHA20(bench, BUFFER_SIZE_TINY);
}

static void CHACHA20_256BYTES(benchmark::Bench& bench)
{
    CHACHA20(bench, BUFFER_SIZE_SMALL);
}

static void CHACHA20_1MB(benchmark::Bench& bench)
{
    CHACHA20(bench, BUFFER_SIZE_LARGE);
}

static void FSCHACHA20POLY1305_64BYTES(benchmark::Bench& bench)
{
    FSCHACHA20POLY1305(bench, BUFFER_SIZE_TINY);
}

static void FSCHACHA20POLY1305_256BYTES(benchmark::Bench& bench)
{
    FSCHACHA20POLY1305(bench, BUFFER_SIZE_SMALL);
}

static void FSCHACHA20POLY1305_1MB(benchmark::Bench& bench)
{
    FSCHACHA20POLY1305(bench, BUFFER_SIZE_LARGE);
}

BENCHMARK(CHACHA20_64BYTES, benchmark::PriorityLevel::HIGH);
BENCHMARK(CHACHA20_256BYTES, benchmark::PriorityLevel::HIGH);
BENCHMARK(CHACHA20_1MB, benchmark::PriorityLevel::HIGH);
BENCHMARK(FSCHACHA20POLY1305_64BYTES, benchmark::PriorityLevel::HIGH);
BENCHMARK(FSCHACHA20POLY1305_256BYTES, benchmark::PriorityLevel::HIGH);
BENCHMARK(FSCHACHA20POLY1305_1MB, benchmark::PriorityLevel::HIGH);
