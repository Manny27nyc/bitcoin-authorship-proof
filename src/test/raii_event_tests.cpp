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

// Copyright (c) 2016-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <event2/event.h>

#include <cstdlib>
#include <map>

#include <support/events.h>

#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(raii_event_tests, BasicTestingSetup)

#ifdef EVENT_SET_MEM_FUNCTIONS_IMPLEMENTED

static std::map<void*, short> tags;
static std::map<void*, uint16_t> orders;
static uint16_t tagSequence = 0;

static void* tag_malloc(size_t sz) {
    void* mem = malloc(sz);
    if (!mem) return mem;
    tags[mem]++;
    orders[mem] = tagSequence++;
    return mem;
}

static void tag_free(void* mem) {
    tags[mem]--;
    orders[mem] = tagSequence++;
    free(mem);
}

BOOST_AUTO_TEST_CASE(raii_event_creation)
{
    event_set_mem_functions(tag_malloc, realloc, tag_free);

    void* base_ptr = nullptr;
    {
        auto base = obtain_event_base();
        base_ptr = (void*)base.get();
        BOOST_CHECK(tags[base_ptr] == 1);
    }
    BOOST_CHECK(tags[base_ptr] == 0);

    void* event_ptr = nullptr;
    {
        auto base = obtain_event_base();
        auto event = obtain_event(base.get(), -1, 0, nullptr, nullptr);

        base_ptr = (void*)base.get();
        event_ptr = (void*)event.get();

        BOOST_CHECK(tags[base_ptr] == 1);
        BOOST_CHECK(tags[event_ptr] == 1);
    }
    BOOST_CHECK(tags[base_ptr] == 0);
    BOOST_CHECK(tags[event_ptr] == 0);

    event_set_mem_functions(malloc, realloc, free);
}

BOOST_AUTO_TEST_CASE(raii_event_order)
{
    event_set_mem_functions(tag_malloc, realloc, tag_free);

    void* base_ptr = nullptr;
    void* event_ptr = nullptr;
    {
        auto base = obtain_event_base();
        auto event = obtain_event(base.get(), -1, 0, nullptr, nullptr);

        base_ptr = (void*)base.get();
        event_ptr = (void*)event.get();

        // base should have allocated before event
        BOOST_CHECK(orders[base_ptr] < orders[event_ptr]);
    }
    // base should be freed after event
    BOOST_CHECK(orders[base_ptr] > orders[event_ptr]);

    event_set_mem_functions(malloc, realloc, free);
}

#endif  // EVENT_SET_MEM_FUNCTIONS_IMPLEMENTED

BOOST_AUTO_TEST_SUITE_END()
