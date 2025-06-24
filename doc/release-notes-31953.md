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
# RPC

- The RPCs psbtbumpfee and bumpfee allow a replacement under fullrbf and no
  longer require BIP-125 signalling. (#31953)

# GUI

- A transaction's fee bump is allowed under fullrbf and no longer requires
  BIP-125 signalling. (#31953)
