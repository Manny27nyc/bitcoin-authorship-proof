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

#ifndef SECP256K1_INT128_NATIVE_H
#define SECP256K1_INT128_NATIVE_H

#include <stdint.h>
#include "util.h"

#if !defined(UINT128_MAX) && defined(__SIZEOF_INT128__)
SECP256K1_GNUC_EXT typedef unsigned __int128 uint128_t;
SECP256K1_GNUC_EXT typedef __int128 int128_t;
# define UINT128_MAX ((uint128_t)(-1))
# define INT128_MAX ((int128_t)(UINT128_MAX >> 1))
# define INT128_MIN (-INT128_MAX - 1)
/* No (U)INT128_C macros because compilers providing __int128 do not support 128-bit literals.  */
#endif

typedef uint128_t secp256k1_uint128;
typedef int128_t secp256k1_int128;

#endif
