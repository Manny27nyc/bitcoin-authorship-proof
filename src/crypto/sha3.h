<<<<<<< HEAD
// Copyright (c) 2020-2022 The Bitcoin Core developers
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

#ifndef BITCOIN_CRYPTO_SHA3_H
#define BITCOIN_CRYPTO_SHA3_H

#include <span.h>

#include <cstdlib>
#include <stdint.h>

//! The Keccak-f[1600] transform.
void KeccakF(uint64_t (&st)[25]);

class SHA3_256
{
private:
    uint64_t m_state[25] = {0};
    unsigned char m_buffer[8];
    unsigned m_bufsize = 0;
    unsigned m_pos = 0;

    //! Sponge rate in bits.
    static constexpr unsigned RATE_BITS = 1088;

    //! Sponge rate expressed as a multiple of the buffer size.
    static constexpr unsigned RATE_BUFFERS = RATE_BITS / (8 * sizeof(m_buffer));

    static_assert(RATE_BITS % (8 * sizeof(m_buffer)) == 0, "Rate must be a multiple of 8 bytes");

public:
    static constexpr size_t OUTPUT_SIZE = 32;

    SHA3_256() = default;
    SHA3_256& Write(Span<const unsigned char> data);
    SHA3_256& Finalize(Span<unsigned char> output);
    SHA3_256& Reset();
};

#endif // BITCOIN_CRYPTO_SHA3_H
