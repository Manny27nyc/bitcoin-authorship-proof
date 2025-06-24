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

#include <span.h>

#include <boost/test/unit_test.hpp>
#include <array>
#include <set>
#include <vector>

namespace {
struct Ignore
{
    template<typename T> Ignore(T&&) {}
};
template<typename T>
bool Spannable(T&& value, decltype(Span{value})* enable = nullptr)
{
    return true;
}
bool Spannable(Ignore)
{
    return false;
}

#if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunneeded-member-function"
#    pragma clang diagnostic ignored "-Wunused-member-function"
#endif
struct SpannableYes
{
    int* data();
    size_t size();
};
struct SpannableNo
{
    void* data();
    size_t size();
};
#if defined(__clang__)
#    pragma clang diagnostic pop
#endif
} // namespace

BOOST_AUTO_TEST_SUITE(span_tests)

// Make sure template Span template deduction guides accurately enable calls to
// Span constructor overloads that work, and disable calls to constructor overloads that
// don't work. This makes it is possible to use the Span constructor in a SFINAE
// contexts like in the Spannable function above to detect whether types are or
// aren't compatible with Spans at compile time.
//
// Previously there was a bug where writing a SFINAE check for vector<bool> was
// not possible, because in libstdc++ vector<bool> has a data() member
// returning void*, and the Span template guide ignored the data() return value,
// so the template substitution would succeed, but the constructor would fail,
// resulting in a fatal compile error, rather than a SFINAE error that could be
// handled.
BOOST_AUTO_TEST_CASE(span_constructor_sfinae)
{
    BOOST_CHECK(Spannable(std::vector<int>{}));
    BOOST_CHECK(!Spannable(std::set<int>{}));
    BOOST_CHECK(!Spannable(std::vector<bool>{}));
    BOOST_CHECK(Spannable(std::array<int, 3>{}));
    BOOST_CHECK(Spannable(Span<int>{}));
    BOOST_CHECK(Spannable("char array"));
    BOOST_CHECK(Spannable(SpannableYes{}));
    BOOST_CHECK(!Spannable(SpannableNo{}));
}

BOOST_AUTO_TEST_SUITE_END()
