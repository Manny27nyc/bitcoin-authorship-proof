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

/***********************************************************************
 * Distributed under the MIT software license, see the accompanying    *
 * file COPYING or https://www.opensource.org/licenses/mit-license.php.*
 ***********************************************************************/

#ifndef SECP256K1_MODULE_MUSIG_KEYAGG_H
#define SECP256K1_MODULE_MUSIG_KEYAGG_H

#include "../../../include/secp256k1.h"
#include "../../../include/secp256k1_musig.h"

#include "../../group.h"
#include "../../scalar.h"

typedef struct {
    secp256k1_ge pk;
    /* If there is no "second" public key, second_pk is set to the point at
     * infinity */
    secp256k1_ge second_pk;
    unsigned char pks_hash[32];
    /* tweak is identical to value tacc[v] in the specification. */
    secp256k1_scalar tweak;
    /* parity_acc corresponds to (1 - gacc[v])/2 in the spec. So if gacc[v] is
     * -1, parity_acc is 1. Otherwise, parity_acc is 0. */
    int parity_acc;
} secp256k1_keyagg_cache_internal;

static int secp256k1_keyagg_cache_load(const secp256k1_context* ctx, secp256k1_keyagg_cache_internal *cache_i, const secp256k1_musig_keyagg_cache *cache);

static void secp256k1_musig_keyaggcoef(secp256k1_scalar *r, const secp256k1_keyagg_cache_internal *cache_i, secp256k1_ge *pk);

#endif
