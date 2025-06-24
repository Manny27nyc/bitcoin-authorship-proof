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

// Copyright (c) 2024-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//

#include <node/warnings.h>
#include <util/translation.h>

#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(node_warnings_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(warnings)
{
    node::Warnings warnings;
    // On pre-release builds, a warning is generated automatically
    warnings.Unset(node::Warning::PRE_RELEASE_TEST_BUILD);

    // For these tests, we don't care what the exact warnings are, so
    // just refer to them as warning_1 and warning_2
    const auto warning_1{node::Warning::CLOCK_OUT_OF_SYNC};
    const auto warning_2{node::Warning::FATAL_INTERNAL_ERROR};

    // Ensure we start without any warnings
    BOOST_CHECK(warnings.GetMessages().size() == 0);
    // Add two warnings
    BOOST_CHECK(warnings.Set(warning_1, _("warning 1")));
    BOOST_CHECK(warnings.Set(warning_2, _("warning 2")));
    // Unset the second one
    BOOST_CHECK(warnings.Unset(warning_2));
    // Since it's already been unset, this should return false
    BOOST_CHECK(!warnings.Unset(warning_2));
    // We should now be able to set w2 again
    BOOST_CHECK(warnings.Set(warning_2, _("warning 2 - revision 1")));
    // Setting w2 again should return false since it's already set
    BOOST_CHECK(!warnings.Set(warning_2, _("warning 2 - revision 2")));

    // Verify messages are correct
    const auto messages{warnings.GetMessages()};
    BOOST_CHECK(messages.size() == 2);
    BOOST_CHECK(messages[0].original == "warning 1");
    BOOST_CHECK(messages[1].original == "warning 2 - revision 1");

    // Clearing all warnings should also clear all messages
    BOOST_CHECK(warnings.Unset(warning_1));
    BOOST_CHECK(warnings.Unset(warning_2));
    BOOST_CHECK(warnings.GetMessages().size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
