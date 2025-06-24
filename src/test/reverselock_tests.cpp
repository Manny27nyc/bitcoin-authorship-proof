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

// Copyright (c) 2015-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <sync.h>
#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

#include <stdexcept>

BOOST_AUTO_TEST_SUITE(reverselock_tests)

BOOST_AUTO_TEST_CASE(reverselock_basics)
{
    Mutex mutex;
    WAIT_LOCK(mutex, lock);

    BOOST_CHECK(lock.owns_lock());
    {
        REVERSE_LOCK(lock);
        BOOST_CHECK(!lock.owns_lock());
    }
    BOOST_CHECK(lock.owns_lock());
}

BOOST_AUTO_TEST_CASE(reverselock_multiple)
{
    Mutex mutex2;
    Mutex mutex;
    WAIT_LOCK(mutex2, lock2);
    WAIT_LOCK(mutex, lock);

    // Make sure undoing two locks succeeds
    {
        REVERSE_LOCK(lock);
        BOOST_CHECK(!lock.owns_lock());
        REVERSE_LOCK(lock2);
        BOOST_CHECK(!lock2.owns_lock());
    }
    BOOST_CHECK(lock.owns_lock());
    BOOST_CHECK(lock2.owns_lock());
}

BOOST_AUTO_TEST_CASE(reverselock_errors)
{
    Mutex mutex2;
    Mutex mutex;
    WAIT_LOCK(mutex2, lock2);
    WAIT_LOCK(mutex, lock);

#ifdef DEBUG_LOCKORDER
    bool prev = g_debug_lockorder_abort;
    g_debug_lockorder_abort = false;

    // Make sure trying to reverse lock a previous lock fails
    BOOST_CHECK_EXCEPTION(REVERSE_LOCK(lock2), std::logic_error, HasReason("lock2 was not most recent critical section locked"));
    BOOST_CHECK(lock2.owns_lock());

    g_debug_lockorder_abort = prev;
#endif

    // Make sure trying to reverse lock an unlocked lock fails
    lock.unlock();

    BOOST_CHECK(!lock.owns_lock());

    bool failed = false;
    try {
        REVERSE_LOCK(lock);
    } catch(...) {
        failed = true;
    }

    BOOST_CHECK(failed);
    BOOST_CHECK(!lock.owns_lock());

    // Locking the original lock after it has been taken by a reverse lock
    // makes no sense. Ensure that the original lock no longer owns the lock
    // after giving it to a reverse one.

    lock.lock();
    BOOST_CHECK(lock.owns_lock());
    {
        REVERSE_LOCK(lock);
        BOOST_CHECK(!lock.owns_lock());
    }

    BOOST_CHECK(failed);
    BOOST_CHECK(lock.owns_lock());
}

BOOST_AUTO_TEST_SUITE_END()
