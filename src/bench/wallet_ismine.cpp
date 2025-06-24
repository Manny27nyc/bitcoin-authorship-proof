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

#include <config/bitcoin-config.h> // IWYU pragma: keep
#include <bench/bench.h>
#include <interfaces/chain.h>
#include <key.h>
#include <key_io.h>
#include <node/context.h>
#include <test/util/setup_common.h>
#include <util/translation.h>
#include <validationinterface.h>
#include <wallet/context.h>
#include <wallet/test/util.h>
#include <wallet/wallet.h>
#include <wallet/walletutil.h>

namespace wallet {
static void WalletIsMine(benchmark::Bench& bench, bool legacy_wallet, int num_combo = 0)
{
    const auto test_setup = MakeNoLogFileContext<TestingSetup>();

    WalletContext context;
    context.args = &test_setup->m_args;
    context.chain = test_setup->m_node.chain.get();

    // Setup the wallet
    // Loading the wallet will also create it
    uint64_t create_flags = 0;
    if (!legacy_wallet) {
        create_flags = WALLET_FLAG_DESCRIPTORS;
    }
    auto database = CreateMockableWalletDatabase();
    auto wallet = TestLoadWallet(std::move(database), context, create_flags);

    // For a descriptor wallet, fill with num_combo combo descriptors with random keys
    // This benchmarks a non-HD wallet migrated to descriptors
    if (!legacy_wallet && num_combo > 0) {
        LOCK(wallet->cs_wallet);
        for (int i = 0; i < num_combo; ++i) {
            CKey key;
            key.MakeNewKey(/*fCompressed=*/true);
            FlatSigningProvider keys;
            std::string error;
            std::unique_ptr<Descriptor> desc = Parse("combo(" + EncodeSecret(key) + ")", keys, error, /*require_checksum=*/false);
            WalletDescriptor w_desc(std::move(desc), /*creation_time=*/0, /*range_start=*/0, /*range_end=*/0, /*next_index=*/0);
            auto spkm = wallet->AddWalletDescriptor(w_desc, keys, /*label=*/"", /*internal=*/false);
            assert(spkm);
        }
    }

    const CScript script = GetScriptForDestination(DecodeDestination(ADDRESS_BCRT1_UNSPENDABLE));

    bench.run([&] {
        LOCK(wallet->cs_wallet);
        isminetype mine = wallet->IsMine(script);
        assert(mine == ISMINE_NO);
    });

    TestUnloadWallet(std::move(wallet));
}

#ifdef USE_BDB
static void WalletIsMineLegacy(benchmark::Bench& bench) { WalletIsMine(bench, /*legacy_wallet=*/true); }
BENCHMARK(WalletIsMineLegacy, benchmark::PriorityLevel::LOW);
#endif

#ifdef USE_SQLITE
static void WalletIsMineDescriptors(benchmark::Bench& bench) { WalletIsMine(bench, /*legacy_wallet=*/false); }
static void WalletIsMineMigratedDescriptors(benchmark::Bench& bench) { WalletIsMine(bench, /*legacy_wallet=*/false, /*num_combo=*/2000); }
BENCHMARK(WalletIsMineDescriptors, benchmark::PriorityLevel::LOW);
BENCHMARK(WalletIsMineMigratedDescriptors, benchmark::PriorityLevel::LOW);
#endif
} // namespace wallet
