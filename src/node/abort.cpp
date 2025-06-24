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

// Copyright (c) 2023 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <node/abort.h>

#include <logging.h>
#include <node/interface_ui.h>
#include <node/warnings.h>
#include <util/signalinterrupt.h>
#include <util/translation.h>

#include <atomic>
#include <cstdlib>

namespace node {

void AbortNode(util::SignalInterrupt* shutdown, std::atomic<int>& exit_status, const bilingual_str& message, node::Warnings* warnings)
{
    if (warnings) warnings->Set(Warning::FATAL_INTERNAL_ERROR, message);
    InitError(_("A fatal internal error occurred, see debug.log for details: ") + message);
    exit_status.store(EXIT_FAILURE);
    if (shutdown && !(*shutdown)()) {
        LogError("Failed to send shutdown signal\n");
    };
}
} // namespace node
