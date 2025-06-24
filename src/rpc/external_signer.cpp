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

// Copyright (c) 2018-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <config/bitcoin-config.h> // IWYU pragma: keep

#include <common/args.h>
#include <common/system.h>
#include <external_signer.h>
#include <rpc/protocol.h>
#include <rpc/server.h>
#include <rpc/util.h>
#include <util/strencodings.h>

#include <string>
#include <vector>

#ifdef ENABLE_EXTERNAL_SIGNER

static RPCHelpMan enumeratesigners()
{
    return RPCHelpMan{"enumeratesigners",
        "Returns a list of external signers from -signer.",
        {},
        RPCResult{
            RPCResult::Type::OBJ, "", "",
            {
                {RPCResult::Type::ARR, "signers", /*optional=*/false, "",
                {
                    {RPCResult::Type::OBJ, "", "",
                    {
                        {RPCResult::Type::STR_HEX, "fingerprint", "Master key fingerprint"},
                        {RPCResult::Type::STR, "name", "Device name"},
                    }},
                },
                }
            }
        },
        RPCExamples{
            HelpExampleCli("enumeratesigners", "")
            + HelpExampleRpc("enumeratesigners", "")
        },
        [&](const RPCHelpMan& self, const JSONRPCRequest& request) -> UniValue
        {
            const std::string command = gArgs.GetArg("-signer", "");
            if (command == "") throw JSONRPCError(RPC_MISC_ERROR, "Error: restart bitcoind with -signer=<cmd>");
            const std::string chain = gArgs.GetChainTypeString();
            UniValue signers_res = UniValue::VARR;
            try {
                std::vector<ExternalSigner> signers;
                ExternalSigner::Enumerate(command, signers, chain);
                for (const ExternalSigner& signer : signers) {
                    UniValue signer_res = UniValue::VOBJ;
                    signer_res.pushKV("fingerprint", signer.m_fingerprint);
                    signer_res.pushKV("name", signer.m_name);
                    signers_res.push_back(std::move(signer_res));
                }
            } catch (const std::exception& e) {
                throw JSONRPCError(RPC_MISC_ERROR, e.what());
            }
            UniValue result(UniValue::VOBJ);
            result.pushKV("signers", std::move(signers_res));
            return result;
        }
    };
}

void RegisterSignerRPCCommands(CRPCTable& t)
{
    static const CRPCCommand commands[]{
        {"signer", &enumeratesigners},
    };
    for (const auto& c : commands) {
        t.appendCommand(c.name, &c);
    }
}

#endif // ENABLE_EXTERNAL_SIGNER
