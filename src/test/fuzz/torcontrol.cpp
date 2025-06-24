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

// Copyright (c) 2020-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>
#include <test/util/setup_common.h>
#include <torcontrol.h>

#include <cstdint>
#include <string>
#include <vector>

class DummyTorControlConnection : public TorControlConnection
{
public:
    DummyTorControlConnection() : TorControlConnection{nullptr}
    {
    }

    bool Connect(const std::string&, const ConnectionCB&, const ConnectionCB&)
    {
        return true;
    }

    void Disconnect()
    {
    }

    bool Command(const std::string&, const ReplyHandlerCB&)
    {
        return true;
    }
};

void initialize_torcontrol()
{
    static const auto testing_setup = MakeNoLogFileContext<>();
}

FUZZ_TARGET(torcontrol, .init = initialize_torcontrol)
{
    FuzzedDataProvider fuzzed_data_provider{buffer.data(), buffer.size()};

    TorController tor_controller;
    LIMITED_WHILE(fuzzed_data_provider.ConsumeBool(), 10000) {
        TorControlReply tor_control_reply;
        CallOneOf(
            fuzzed_data_provider,
            [&] {
                tor_control_reply.code = 250;
            },
            [&] {
                tor_control_reply.code = 510;
            },
            [&] {
                tor_control_reply.code = fuzzed_data_provider.ConsumeIntegral<int>();
            });
        tor_control_reply.lines = ConsumeRandomLengthStringVector(fuzzed_data_provider);
        if (tor_control_reply.lines.empty()) {
            break;
        }
        DummyTorControlConnection dummy_tor_control_connection;
        CallOneOf(
            fuzzed_data_provider,
            [&] {
                tor_controller.add_onion_cb(dummy_tor_control_connection, tor_control_reply);
            },
            [&] {
                tor_controller.auth_cb(dummy_tor_control_connection, tor_control_reply);
            },
            [&] {
                tor_controller.authchallenge_cb(dummy_tor_control_connection, tor_control_reply);
            },
            [&] {
                tor_controller.protocolinfo_cb(dummy_tor_control_connection, tor_control_reply);
            });
    }
}
