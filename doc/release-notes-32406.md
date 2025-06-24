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
- `-datacarriersize` is increased to 100,000 which effectively uncaps the limit (as the maximum transaction size limit will be hit first). It can be overridden with -datacarriersize=83 to revert to the limit enforced in previous versions. Both `-datacarrier` and `-datacarriersize` options have been marked as deprecated and are expected to be removed in a future release. (#32406)

- Multiple data carrier (OP_RETURN) outputs in a transaction are now permitted for relay and mining. The `-datacarriersize` limit applies to the aggregate size of the scriptPubKeys across all such outputs in a transaction, not including the scriptPubKey size itself. (#32406)
