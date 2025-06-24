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

// Copyright (c) 2019-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/consensus.h>
#include <core_io.h>
#include <policy/policy.h>
#include <script/script.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>
#include <univalue.h>
#include <util/chaintype.h>

void initialize_script_format()
{
    SelectParams(ChainType::REGTEST);
}

FUZZ_TARGET(script_format, .init = initialize_script_format)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    const CScript script{ConsumeScript(fuzzed_data_provider)};
    if (script.size() > MAX_STANDARD_TX_WEIGHT / WITNESS_SCALE_FACTOR) {
        return;
    }

    (void)FormatScript(script);
    (void)ScriptToAsmStr(script, /*fAttemptSighashDecode=*/fuzzed_data_provider.ConsumeBool());

    UniValue o1(UniValue::VOBJ);
    auto include_hex = fuzzed_data_provider.ConsumeBool();
    auto include_address = fuzzed_data_provider.ConsumeBool();
    ScriptToUniv(script, /*out=*/o1, include_hex, include_address);
}
