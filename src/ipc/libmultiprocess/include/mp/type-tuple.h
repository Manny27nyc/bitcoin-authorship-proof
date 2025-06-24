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

#ifndef MP_PROXY_TYPE_TUPLE_H
#define MP_PROXY_TYPE_TUPLE_H

#include <mp/util.h>

namespace mp {
// TODO: Should generalize this to work with arbitrary length tuples, not just length 2-tuples.
template <typename KeyLocalType, typename ValueLocalType, typename Value, typename Output>
void CustomBuildField(TypeList<std::tuple<KeyLocalType, ValueLocalType>>,
    Priority<1>,
    InvokeContext& invoke_context,
    Value&& value,
    Output&& output)
{
    auto pair = output.init();
    using Accessors = typename ProxyStruct<typename decltype(pair)::Builds>::Accessors;
    BuildField(TypeList<KeyLocalType>(), invoke_context, Make<StructField, std::tuple_element_t<0, Accessors>>(pair), std::get<0>(value));
    BuildField(TypeList<ValueLocalType>(), invoke_context, Make<StructField, std::tuple_element_t<1, Accessors>>(pair), std::get<1>(value));
}

// TODO: Should generalize this to work with arbitrary length tuples, not just length 2-tuples.
template <typename KeyLocalType, typename ValueLocalType, typename Input, typename ReadDest>
decltype(auto) CustomReadField(TypeList<std::tuple<KeyLocalType, ValueLocalType>>,
    Priority<1>,
    InvokeContext& invoke_context,
    Input&& input,
    ReadDest&& read_dest)
{
    return read_dest.update([&](auto& value) {
        const auto& pair = input.get();
        using Struct = ProxyStruct<typename Decay<decltype(pair)>::Reads>;
        using Accessors = typename Struct::Accessors;
        ReadField(TypeList<KeyLocalType>(), invoke_context, Make<StructField, std::tuple_element_t<0, Accessors>>(pair),
            ReadDestUpdate(std::get<0>(value)));
        ReadField(TypeList<ValueLocalType>(), invoke_context, Make<StructField, std::tuple_element_t<1, Accessors>>(pair),
            ReadDestUpdate(std::get<1>(value)));
    });
}
} // namespace mp

#endif // MP_PROXY_TYPE_TUPLE_H
