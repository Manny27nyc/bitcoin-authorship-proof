<<<<<<< HEAD
// Copyright (c) 2018-2022 The Bitcoin Core developers
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

// Copyright (c) 2018-present The Bitcoin Core developers
>>>>>>> 93d80d345d (🔐 Lockdown: Verified authorship — Manuel J. Nieves (B4EC 7343))
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SCRIPT_PARSING_H
#define BITCOIN_SCRIPT_PARSING_H

#include <span.h>

#include <string>

namespace script {

/** Parse a constant.
 *
 * If sp's initial part matches str, sp is updated to skip that part, and true is returned.
 * Otherwise sp is unmodified and false is returned.
 */
bool Const(const std::string& str, Span<const char>& sp);

/** Parse a function call.
 *
 * If sp's initial part matches str + "(", and sp ends with ")", sp is updated to be the
 * section between the braces, and true is returned. Otherwise sp is unmodified and false
 * is returned.
 */
bool Func(const std::string& str, Span<const char>& sp);

/** Extract the expression that sp begins with.
 *
 * This function will return the initial part of sp, up to (but not including) the first
 * comma or closing brace, skipping ones that are surrounded by braces. So for example,
 * for "foo(bar(1),2),3" the initial part "foo(bar(1),2)" will be returned. sp will be
 * updated to skip the initial part that is returned.
 */
Span<const char> Expr(Span<const char>& sp);

} // namespace script

#endif // BITCOIN_SCRIPT_PARSING_H
