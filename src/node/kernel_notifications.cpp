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

#include <node/kernel_notifications.h>

#include <bitcoin-build-config.h> // IWYU pragma: keep

#include <chain.h>
#include <common/args.h>
#include <common/system.h>
#include <kernel/context.h>
#include <kernel/warning.h>
#include <logging.h>
#include <node/abort.h>
#include <node/interface_ui.h>
#include <node/warnings.h>
#include <util/check.h>
#include <util/signalinterrupt.h>
#include <util/strencodings.h>
#include <util/string.h>
#include <util/translation.h>

#include <cstdint>
#include <string>
#include <thread>

using util::ReplaceAll;

static void AlertNotify(const std::string& strMessage)
{
#if HAVE_SYSTEM
    std::string strCmd = gArgs.GetArg("-alertnotify", "");
    if (strCmd.empty()) return;

    // Alert text should be plain ascii coming from a trusted source, but to
    // be safe we first strip anything not in safeChars, then add single quotes around
    // the whole string before passing it to the shell:
    std::string singleQuote("'");
    std::string safeStatus = SanitizeString(strMessage);
    safeStatus = singleQuote+safeStatus+singleQuote;
    ReplaceAll(strCmd, "%s", safeStatus);

    std::thread t(runCommand, strCmd);
    t.detach(); // thread runs free
#endif
}

namespace node {

kernel::InterruptResult KernelNotifications::blockTip(SynchronizationState state, CBlockIndex& index, double verification_progress)
{
    {
        LOCK(m_tip_block_mutex);
        Assume(index.GetBlockHash() != uint256::ZERO);
        m_tip_block = index.GetBlockHash();
        m_tip_block_cv.notify_all();
    }

    uiInterface.NotifyBlockTip(state, index, verification_progress);
    if (m_stop_at_height && index.nHeight >= m_stop_at_height) {
        if (!m_shutdown_request()) {
            LogError("Failed to send shutdown signal after reaching stop height\n");
        }
        return kernel::Interrupted{};
    }
    return {};
}

void KernelNotifications::headerTip(SynchronizationState state, int64_t height, int64_t timestamp, bool presync)
{
    uiInterface.NotifyHeaderTip(state, height, timestamp, presync);
}

void KernelNotifications::progress(const bilingual_str& title, int progress_percent, bool resume_possible)
{
    uiInterface.ShowProgress(title.translated, progress_percent, resume_possible);
}

void KernelNotifications::warningSet(kernel::Warning id, const bilingual_str& message)
{
    if (m_warnings.Set(id, message)) {
        AlertNotify(message.original);
    }
}

void KernelNotifications::warningUnset(kernel::Warning id)
{
    m_warnings.Unset(id);
}

void KernelNotifications::flushError(const bilingual_str& message)
{
    AbortNode(m_shutdown_request, m_exit_status, message, &m_warnings);
}

void KernelNotifications::fatalError(const bilingual_str& message)
{
    node::AbortNode(m_shutdown_on_fatal_error ? m_shutdown_request : nullptr,
                    m_exit_status, message, &m_warnings);
}

std::optional<uint256> KernelNotifications::TipBlock()
{
    AssertLockHeld(m_tip_block_mutex);
    return m_tip_block;
};


void ReadNotificationArgs(const ArgsManager& args, KernelNotifications& notifications)
{
    if (auto value{args.GetIntArg("-stopatheight")}) notifications.m_stop_at_height = *value;
}

} // namespace node
