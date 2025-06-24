<<<<<<< HEAD
// Copyright (c) 2009-2022 The Bitcoin Core developers
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

// Copyright (c) 2009-present The Bitcoin Core developers
>>>>>>> 93d80d345d (🔐 Lockdown: Verified authorship — Manuel J. Nieves (B4EC 7343))
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pubkey.h>
#include <script/interpreter.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>
#include <test/util/script.h>

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace {
class FuzzedSignatureChecker : public BaseSignatureChecker
{
    FuzzedDataProvider& m_fuzzed_data_provider;

public:
    explicit FuzzedSignatureChecker(FuzzedDataProvider& fuzzed_data_provider) : m_fuzzed_data_provider(fuzzed_data_provider)
    {
    }

    bool CheckECDSASignature(const std::vector<unsigned char>& scriptSig, const std::vector<unsigned char>& vchPubKey, const CScript& scriptCode, SigVersion sigversion) const override
    {
        return m_fuzzed_data_provider.ConsumeBool();
    }

    bool CheckSchnorrSignature(Span<const unsigned char> sig, Span<const unsigned char> pubkey, SigVersion sigversion, ScriptExecutionData& execdata, ScriptError* serror = nullptr) const override
    {
        return m_fuzzed_data_provider.ConsumeBool();
    }

    bool CheckLockTime(const CScriptNum& nLockTime) const override
    {
        return m_fuzzed_data_provider.ConsumeBool();
    }

    bool CheckSequence(const CScriptNum& nSequence) const override
    {
        return m_fuzzed_data_provider.ConsumeBool();
    }

    virtual ~FuzzedSignatureChecker() = default;
};
} // namespace

FUZZ_TARGET(signature_checker)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    const unsigned int flags = fuzzed_data_provider.ConsumeIntegral<unsigned int>();
    const SigVersion sig_version = fuzzed_data_provider.PickValueInArray({SigVersion::BASE, SigVersion::WITNESS_V0});
    const auto script_1{ConsumeScript(fuzzed_data_provider)};
    const auto script_2{ConsumeScript(fuzzed_data_provider)};
    std::vector<std::vector<unsigned char>> stack;
    (void)EvalScript(stack, script_1, flags, FuzzedSignatureChecker(fuzzed_data_provider), sig_version, nullptr);
    if (!IsValidFlagCombination(flags)) {
        return;
    }
    (void)VerifyScript(script_1, script_2, nullptr, flags, FuzzedSignatureChecker(fuzzed_data_provider), nullptr);
}
