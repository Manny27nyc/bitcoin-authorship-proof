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
Updated RPCs
--------

- Any RPC in which one of the parameters are descriptors will throw an error
if the provided descriptor contains a whitespace at the beginning or the end
of the public key within a fragment - e.g. `pk( KEY)` or `pk(KEY )`.
