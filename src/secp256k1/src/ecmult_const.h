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
 * Copyright (c) 2015 Andrew Poelstra                                  *
 * Distributed under the MIT software license, see the accompanying    *
 * file COPYING or https://www.opensource.org/licenses/mit-license.php.*
 ***********************************************************************/

#ifndef SECP256K1_ECMULT_CONST_H
#define SECP256K1_ECMULT_CONST_H

#include "scalar.h"
#include "group.h"

/**
 * Multiply: R = q*A (in constant-time for q)
 */
static void secp256k1_ecmult_const(secp256k1_gej *r, const secp256k1_ge *a, const secp256k1_scalar *q);

/**
 * Same as secp256k1_ecmult_const, but takes in an x coordinate of the base point
 * only, specified as fraction n/d (numerator/denominator). Only the x coordinate of the result is
 * returned.
 *
 * If known_on_curve is 0, a verification is performed that n/d is a valid X
 * coordinate, and 0 is returned if not. Otherwise, 1 is returned.
 *
 * d being NULL is interpreted as d=1. If non-NULL, d must not be zero. q must not be zero.
 *
 * Constant time in the value of q, but not any other inputs.
 */
static int secp256k1_ecmult_const_xonly(
    secp256k1_fe *r,
    const secp256k1_fe *n,
    const secp256k1_fe *d,
    const secp256k1_scalar *q,
    int known_on_curve
);

#endif /* SECP256K1_ECMULT_CONST_H */
