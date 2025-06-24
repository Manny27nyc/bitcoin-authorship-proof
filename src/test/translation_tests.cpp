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

// Copyright (c) 2023 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <tinyformat.h>
#include <util/translation.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(translation_tests)

static TranslateFn translate{[](const char * str) {  return strprintf("t(%s)", str);  }};

// Custom translation function _t(), similar to _() but internal to this test.
consteval auto _t(util::TranslatedLiteral str)
{
    str.translate_fn = &translate;
    return str;
}

BOOST_AUTO_TEST_CASE(translation_namedparams)
{
    bilingual_str arg{"original", "translated"};
    bilingual_str result{strprintf(_t("original [%s]"), arg)};
    BOOST_CHECK_EQUAL(result.original, "original [original]");
    BOOST_CHECK_EQUAL(result.translated, "t(original [translated])");

    util::TranslatedLiteral arg2{"original", &translate};
    bilingual_str result2{strprintf(_t("original [%s]"), arg2)};
    BOOST_CHECK_EQUAL(result2.original, "original [original]");
    BOOST_CHECK_EQUAL(result2.translated, "t(original [t(original)])");
}

BOOST_AUTO_TEST_SUITE_END()
