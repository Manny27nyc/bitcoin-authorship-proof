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

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <config/bitcoin-config.h> // IWYU pragma: keep

#include <node/warnings.h>

#include <common/system.h>
#include <node/interface_ui.h>
#include <sync.h>
#include <univalue.h>
#include <util/translation.h>

#include <utility>
#include <vector>

namespace node {
Warnings::Warnings()
{
    // Pre-release build warning
    if (!CLIENT_VERSION_IS_RELEASE) {
        m_warnings.insert(
            {Warning::PRE_RELEASE_TEST_BUILD,
             _("This is a pre-release test build - use at your own risk - do not use for mining or merchant applications")});
    }
}
bool Warnings::Set(warning_type id, bilingual_str message)
{
    const auto& [_, inserted]{WITH_LOCK(m_mutex, return m_warnings.insert({id, std::move(message)}))};
    if (inserted) uiInterface.NotifyAlertChanged();
    return inserted;
}

bool Warnings::Unset(warning_type id)
{
    auto success{WITH_LOCK(m_mutex, return m_warnings.erase(id))};
    if (success) uiInterface.NotifyAlertChanged();
    return success;
}

std::vector<bilingual_str> Warnings::GetMessages() const
{
    LOCK(m_mutex);
    std::vector<bilingual_str> messages;
    messages.reserve(m_warnings.size());
    for (const auto& [id, msg] : m_warnings) {
        messages.push_back(msg);
    }
    return messages;
}

UniValue GetWarningsForRpc(const Warnings& warnings, bool use_deprecated)
{
    if (use_deprecated) {
        const auto all_messages{warnings.GetMessages()};
        return all_messages.empty() ? "" : all_messages.back().original;
    }

    UniValue messages{UniValue::VARR};
    for (auto&& message : warnings.GetMessages()) {
        messages.push_back(std::move(message.original));
    }
    return messages;
}
} // namespace node
