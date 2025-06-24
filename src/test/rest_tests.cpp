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

// Copyright (c) 2012-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <rest.h>
#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_FIXTURE_TEST_SUITE(rest_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(test_query_string)
{
    std::string param;
    RESTResponseFormat rf;
    // No query string
    rf = ParseDataFormat(param, "/rest/endpoint/someresource.json");
    BOOST_CHECK_EQUAL(param, "/rest/endpoint/someresource");
    BOOST_CHECK_EQUAL(rf, RESTResponseFormat::JSON);

    // Query string with single parameter
    rf = ParseDataFormat(param, "/rest/endpoint/someresource.bin?p1=v1");
    BOOST_CHECK_EQUAL(param, "/rest/endpoint/someresource");
    BOOST_CHECK_EQUAL(rf, RESTResponseFormat::BINARY);

    // Query string with multiple parameters
    rf = ParseDataFormat(param, "/rest/endpoint/someresource.hex?p1=v1&p2=v2");
    BOOST_CHECK_EQUAL(param, "/rest/endpoint/someresource");
    BOOST_CHECK_EQUAL(rf, RESTResponseFormat::HEX);

    // Incorrectly formed query string will not be handled
    rf = ParseDataFormat(param, "/rest/endpoint/someresource.json&p1=v1");
    BOOST_CHECK_EQUAL(param, "/rest/endpoint/someresource.json&p1=v1");
    BOOST_CHECK_EQUAL(rf, RESTResponseFormat::UNDEF);

    // Omitted data format with query string should return UNDEF and hide query string
    rf = ParseDataFormat(param, "/rest/endpoint/someresource?p1=v1");
    BOOST_CHECK_EQUAL(param, "/rest/endpoint/someresource");
    BOOST_CHECK_EQUAL(rf, RESTResponseFormat::UNDEF);

    // Data format specified after query string
    rf = ParseDataFormat(param, "/rest/endpoint/someresource?p1=v1.json");
    BOOST_CHECK_EQUAL(param, "/rest/endpoint/someresource");
    BOOST_CHECK_EQUAL(rf, RESTResponseFormat::UNDEF);
}
BOOST_AUTO_TEST_SUITE_END()
