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

#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>
#include <test/util/setup_common.h>
#include <wallet/crypter.h>

namespace wallet {
namespace {

const TestingSetup* g_setup;
void initialize_crypter()
{
    static const auto testing_setup = MakeNoLogFileContext<const TestingSetup>();
    g_setup = testing_setup.get();
}

FUZZ_TARGET(crypter, .init = initialize_crypter)
{
    SeedRandomStateForTest(SeedRand::ZEROS);
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    bool good_data{true};

    CCrypter crypt;
    // These values are regularly updated within `CallOneOf`
    std::vector<unsigned char> cipher_text_ed;
    CKeyingMaterial plain_text_ed;
    const std::vector<unsigned char> random_key = ConsumeFixedLengthByteVector(fuzzed_data_provider, WALLET_CRYPTO_KEY_SIZE);

    if (fuzzed_data_provider.ConsumeBool()) {
        const std::string random_string = fuzzed_data_provider.ConsumeRandomLengthString(100);
        SecureString secure_string(random_string.begin(), random_string.end());

        const unsigned int derivation_method = fuzzed_data_provider.ConsumeBool() ? 0 : fuzzed_data_provider.ConsumeIntegral<unsigned int>();

        // Limiting the value of rounds since it is otherwise uselessly expensive and causes a timeout when fuzzing.
        crypt.SetKeyFromPassphrase(/*key_data=*/secure_string,
                                   /*salt=*/ConsumeFixedLengthByteVector(fuzzed_data_provider, WALLET_CRYPTO_SALT_SIZE),
                                   /*rounds=*/fuzzed_data_provider.ConsumeIntegralInRange<unsigned int>(0, CMasterKey::DEFAULT_DERIVE_ITERATIONS),
                                   /*derivation_method=*/derivation_method);
    }

    CKey random_ckey;
    random_ckey.Set(random_key.begin(), random_key.end(), /*fCompressedIn=*/fuzzed_data_provider.ConsumeBool());
    if (!random_ckey.IsValid()) return;
    CPubKey pubkey{random_ckey.GetPubKey()};

    LIMITED_WHILE(good_data && fuzzed_data_provider.ConsumeBool(), 100)
    {
        CallOneOf(
            fuzzed_data_provider,
            [&] {
                const std::vector<unsigned char> random_vector = ConsumeFixedLengthByteVector(fuzzed_data_provider, WALLET_CRYPTO_KEY_SIZE);
                plain_text_ed = CKeyingMaterial(random_vector.begin(), random_vector.end());
            },
            [&] {
                cipher_text_ed = ConsumeRandomLengthByteVector(fuzzed_data_provider, 64);
            },
            [&] {
                (void)crypt.Encrypt(plain_text_ed, cipher_text_ed);
            },
            [&] {
                (void)crypt.Decrypt(cipher_text_ed, plain_text_ed);
            },
            [&] {
                const CKeyingMaterial master_key(random_key.begin(), random_key.end());;
                (void)EncryptSecret(master_key, plain_text_ed, pubkey.GetHash(), cipher_text_ed);
            },
            [&] {
                std::optional<CPubKey> random_pub_key{ConsumeDeserializable<CPubKey>(fuzzed_data_provider)};
                if (!random_pub_key) {
                    good_data = false;
                    return;
                }
                pubkey = *random_pub_key;
            },
            [&] {
                const CKeyingMaterial master_key(random_key.begin(), random_key.end());
                CKey key;
                (void)DecryptKey(master_key, cipher_text_ed, pubkey, key);
            });
    }
}
} // namespace
} // namespace wallet
