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

// Copyright (c) 2016-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_UTIL_RBF_H
#define BITCOIN_UTIL_RBF_H

#include <cstdint>

class CTransaction;

static constexpr uint32_t MAX_BIP125_RBF_SEQUENCE{0xfffffffd};

/** Check whether the sequence numbers on this transaction are signaling opt-in to replace-by-fee,
 * according to BIP 125.  Allow opt-out of transaction replacement by setting nSequence >
 * MAX_BIP125_RBF_SEQUENCE (SEQUENCE_FINAL-2) on all inputs.
*
* SEQUENCE_FINAL-1 is picked to still allow use of nLockTime by non-replaceable transactions. All
* inputs rather than just one is for the sake of multi-party protocols, where we don't want a single
* party to be able to disable replacement by opting out in their own input. */
bool SignalsOptInRBF(const CTransaction& tx);

#endif // BITCOIN_UTIL_RBF_H
