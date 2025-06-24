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

// Copyright (c) 2011-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <util/strencodings.h>

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>

using namespace std::literals;

BOOST_AUTO_TEST_SUITE(base64_tests)

BOOST_AUTO_TEST_CASE(base64_testvectors)
{
    static const std::string vstrIn[]  = {"","f","fo","foo","foob","fooba","foobar"};
    static const std::string vstrOut[] = {"","Zg==","Zm8=","Zm9v","Zm9vYg==","Zm9vYmE=","Zm9vYmFy"};
    for (unsigned int i=0; i<std::size(vstrIn); i++)
    {
        std::string strEnc = EncodeBase64(vstrIn[i]);
        BOOST_CHECK_EQUAL(strEnc, vstrOut[i]);
        auto dec = DecodeBase64(strEnc);
        BOOST_REQUIRE(dec);
        BOOST_CHECK_MESSAGE(std::ranges::equal(*dec, vstrIn[i]), vstrOut[i]);
    }

    {
        const std::vector<uint8_t> in_u{0xff, 0x01, 0xff};
        const std::vector<std::byte> in_b{std::byte{0xff}, std::byte{0x01}, std::byte{0xff}};
        const std::string in_s{"\xff\x01\xff"};
        const std::string out_exp{"/wH/"};
        BOOST_CHECK_EQUAL(EncodeBase64(in_u), out_exp);
        BOOST_CHECK_EQUAL(EncodeBase64(in_b), out_exp);
        BOOST_CHECK_EQUAL(EncodeBase64(in_s), out_exp);
    }

    BOOST_CHECK(DecodeBase64("nQB/pZw=")); // valid

    // Decoding strings with embedded NUL characters should fail
    BOOST_CHECK(!DecodeBase64("invalid\0"sv)); // correct size, invalid due to \0
    BOOST_CHECK(!DecodeBase64("nQB/pZw=\0invalid"sv));
    BOOST_CHECK(!DecodeBase64("nQB/pZw=invalid\0"sv)); // invalid, padding only allowed at the end
}

BOOST_AUTO_TEST_CASE(base64_padding)
{
    // Is valid without padding
    BOOST_CHECK_EQUAL(EncodeBase64("foobar"), "Zm9vYmFy");

    // Valid size
    BOOST_CHECK(!DecodeBase64("===="));
    BOOST_CHECK(!DecodeBase64("a==="));
    BOOST_CHECK( DecodeBase64("YQ=="));
    BOOST_CHECK( DecodeBase64("YWE="));
}

BOOST_AUTO_TEST_SUITE_END()
