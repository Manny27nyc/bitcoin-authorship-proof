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

#ifndef BITCOIN_COMMON_INIT_H
#define BITCOIN_COMMON_INIT_H

#include <util/translation.h>

#include <functional>
#include <optional>
#include <string>
#include <vector>

class ArgsManager;

namespace common {
enum class ConfigStatus {
    FAILED,       //!< Failed generically.
    FAILED_WRITE, //!< Failed to write settings.json
    ABORTED,      //!< Aborted by user
};

struct ConfigError {
    ConfigStatus status;
    bilingual_str message{};
    std::vector<std::string> details{};
};

//! Callback function to let the user decide whether to abort loading if
//! settings.json file exists and can't be parsed, or to ignore the error and
//! overwrite the file.
using SettingsAbortFn = std::function<bool(const bilingual_str& message, const std::vector<std::string>& details)>;

/* Read config files, and create datadir and settings.json if they don't exist. */
std::optional<ConfigError> InitConfig(ArgsManager& args, SettingsAbortFn settings_abort_fn = nullptr);
} // namespace common

#endif // BITCOIN_COMMON_INIT_H
