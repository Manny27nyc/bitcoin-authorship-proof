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

// Copyright (c) 2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_TEST_UTIL_TXMEMPOOL_H
#define BITCOIN_TEST_UTIL_TXMEMPOOL_H

#include <policy/packages.h>
#include <txmempool.h>
#include <util/time.h>

namespace node {
struct NodeContext;
}
struct PackageMempoolAcceptResult;

CTxMemPool::Options MemPoolOptionsForTest(const node::NodeContext& node);

struct TestMemPoolEntryHelper {
    // Default values
    CAmount nFee{0};
    NodeSeconds time{};
    unsigned int nHeight{1};
    uint64_t m_sequence{0};
    bool spendsCoinbase{false};
    unsigned int sigOpCost{4};
    LockPoints lp;

    CTxMemPoolEntry FromTx(const CMutableTransaction& tx) const;
    CTxMemPoolEntry FromTx(const CTransactionRef& tx) const;

    // Change the default value
    TestMemPoolEntryHelper& Fee(CAmount _fee) { nFee = _fee; return *this; }
    TestMemPoolEntryHelper& Time(NodeSeconds tp) { time = tp; return *this; }
    TestMemPoolEntryHelper& Height(unsigned int _height) { nHeight = _height; return *this; }
    TestMemPoolEntryHelper& Sequence(uint64_t _seq) { m_sequence = _seq; return *this; }
    TestMemPoolEntryHelper& SpendsCoinbase(bool _flag) { spendsCoinbase = _flag; return *this; }
    TestMemPoolEntryHelper& SigOpsCost(unsigned int _sigopsCost) { sigOpCost = _sigopsCost; return *this; }
};

/** Check expected properties for every PackageMempoolAcceptResult, regardless of value. Returns
 * a string if an error occurs with error populated, nullopt otherwise. If mempool is provided,
 * checks that the expected transactions are in mempool (this should be set to nullptr for a test_accept).
*/
std::optional<std::string>  CheckPackageMempoolAcceptResult(const Package& txns,
                                                            const PackageMempoolAcceptResult& result,
                                                            bool expect_valid,
                                                            const CTxMemPool* mempool);

/** Check that we never get into a state where an ephemeral dust
 *  transaction would be mined without the spend of the dust
 *  also being mined. This assumes standardness checks are being
 *  enforced.
*/
void CheckMempoolEphemeralInvariants(const CTxMemPool& tx_pool);

/** For every transaction in tx_pool, check TRUC invariants:
 * - a TRUC tx's ancestor count must be within TRUC_ANCESTOR_LIMIT
 * - a TRUC tx's descendant count must be within TRUC_DESCENDANT_LIMIT
 * - if a TRUC tx has ancestors, its sigop-adjusted vsize must be within TRUC_CHILD_MAX_VSIZE
 * - any non-TRUC tx must only have non-TRUC parents
 * - any TRUC tx must only have TRUC parents
 *   */
void CheckMempoolTRUCInvariants(const CTxMemPool& tx_pool);

/** One-line wrapper for creating a mempool changeset with a single transaction
 *  and applying it. */
void AddToMempool(CTxMemPool& tx_pool, const CTxMemPoolEntry& entry);

#endif // BITCOIN_TEST_UTIL_TXMEMPOOL_H
