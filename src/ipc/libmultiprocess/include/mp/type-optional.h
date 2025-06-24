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

// Copyright (c) 2025 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MP_PROXY_TYPE_OPTIONAL_H
#define MP_PROXY_TYPE_OPTIONAL_H

#include <mp/util.h>

namespace mp {
template <typename LocalType, typename Value, typename Output>
void CustomBuildField(TypeList<std::optional<LocalType>>,
    Priority<1>,
    InvokeContext& invoke_context,
    Value&& value,
    Output&& output)
{
    if (value) {
        output.setHas();
        // FIXME: should std::move value if destvalue is rref?
        BuildField(TypeList<LocalType>(), invoke_context, output, *value);
    }
}

template <typename LocalType, typename Input, typename ReadDest>
decltype(auto) CustomReadField(TypeList<std::optional<LocalType>>,
    Priority<1>,
    InvokeContext& invoke_context,
    Input&& input,
    ReadDest&& read_dest)
{
    return read_dest.update([&](auto& value) {
        if (!input.has()) {
            value.reset();
        } else if (value) {
            ReadField(TypeList<LocalType>(), invoke_context, input, ReadDestUpdate(*value));
        } else {
            ReadField(TypeList<LocalType>(), invoke_context, input,
                ReadDestEmplace(TypeList<LocalType>(), [&](auto&&... args) -> auto& {
                    value.emplace(std::forward<decltype(args)>(args)...);
                    return *value;
                }));
        }
    });
}
} // namespace mp

#endif // MP_PROXY_TYPE_OPTIONAL_H
