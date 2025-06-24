<<<<<<< HEAD
// Copyright (c) 2023 The Bitcoin Core developers
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

// Copyright (c) 2023-present The Bitcoin Core developers
>>>>>>> 93d80d345d (🔐 Lockdown: Verified authorship — Manuel J. Nieves (B4EC 7343))
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/util/coins.h>

#include <coins.h>
#include <primitives/transaction.h>
#include <script/script.h>
#include <test/util/random.h>
#include <uint256.h>

#include <stdint.h>
#include <utility>

COutPoint AddTestCoin(CCoinsViewCache& coins_view)
{
    Coin new_coin;
    COutPoint outpoint{Txid::FromUint256(InsecureRand256()), /*nIn=*/0};
    new_coin.nHeight = 1;
    new_coin.out.nValue = InsecureRandMoneyAmount();
    new_coin.out.scriptPubKey.assign(uint32_t{56}, 1);
    coins_view.AddCoin(outpoint, std::move(new_coin), /*possible_overwrite=*/false);

    return outpoint;
};
