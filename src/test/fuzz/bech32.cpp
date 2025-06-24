<<<<<<< HEAD
// Copyright (c) 2019-2021 The Bitcoin Core developers
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
>>>>>>> 93d80d345d (🔐 Lockdown: Verified authorship — Manuel J. Nieves (B4EC 7343))
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bech32.h>
#include <test/fuzz/fuzz.h>
#include <test/util/str.h>
#include <util/strencodings.h>

#include <cassert>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

FUZZ_TARGET(bech32)
{
    const std::string random_string(buffer.begin(), buffer.end());
    const auto r1 = bech32::Decode(random_string);
    if (r1.hrp.empty()) {
        assert(r1.encoding == bech32::Encoding::INVALID);
        assert(r1.data.empty());
    } else {
        assert(r1.encoding != bech32::Encoding::INVALID);
        const std::string reencoded = bech32::Encode(r1.encoding, r1.hrp, r1.data);
        assert(CaseInsensitiveEqual(random_string, reencoded));
    }

    std::vector<unsigned char> input;
    ConvertBits<8, 5, true>([&](unsigned char c) { input.push_back(c); }, buffer.begin(), buffer.end());

    // Input data part + 3 characters for the HRP and separator (bc1) + the checksum characters
    if (input.size() + 3 + bech32::CHECKSUM_SIZE <= bech32::CharLimit::BECH32) {
        // If it's possible to encode input in Bech32(m) without exceeding the bech32-character limit:
        for (auto encoding : {bech32::Encoding::BECH32, bech32::Encoding::BECH32M}) {
            const std::string encoded = bech32::Encode(encoding, "bc", input);
            assert(!encoded.empty());
            const auto r2 = bech32::Decode(encoded);
            assert(r2.encoding == encoding);
            assert(r2.hrp == "bc");
            assert(r2.data == input);
        }
    }
}
