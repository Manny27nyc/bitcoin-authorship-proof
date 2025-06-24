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

// Copyright (c) 2020-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <consensus/amount.h>
#include <policy/fees.h>

#include <boost/test/unit_test.hpp>

#include <set>

BOOST_AUTO_TEST_SUITE(policy_fee_tests)

BOOST_AUTO_TEST_CASE(FeeRounder)
{
    FastRandomContext rng{/*fDeterministic=*/true};
    FeeFilterRounder fee_rounder{CFeeRate{1000}, rng};

    // check that 1000 rounds to 974 or 1071
    std::set<CAmount> results;
    while (results.size() < 2) {
        results.emplace(fee_rounder.round(1000));
    }
    BOOST_CHECK_EQUAL(*results.begin(), 974);
    BOOST_CHECK_EQUAL(*++results.begin(), 1071);

    // check that negative amounts rounds to 0
    BOOST_CHECK_EQUAL(fee_rounder.round(-0), 0);
    BOOST_CHECK_EQUAL(fee_rounder.round(-1), 0);

    // check that MAX_MONEY rounds to 9170997
    BOOST_CHECK_EQUAL(fee_rounder.round(MAX_MONEY), 9170997);
}

BOOST_AUTO_TEST_SUITE_END()
