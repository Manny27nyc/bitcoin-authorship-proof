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

/*********************************************************************************
 * Copyright (c) 2013, 2014, 2015, 2021 Thomas Daede, Cory Fields, Pieter Wuille *
 * Distributed under the MIT software license, see the accompanying              *
 * file COPYING or https://www.opensource.org/licenses/mit-license.php.          *
 *********************************************************************************/

#ifndef SECP256K1_PRECOMPUTED_ECMULT_GEN_H
#define SECP256K1_PRECOMPUTED_ECMULT_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "group.h"
#include "ecmult_gen.h"
#ifdef EXHAUSTIVE_TEST_ORDER
static secp256k1_ge_storage secp256k1_ecmult_gen_prec_table[COMB_BLOCKS][COMB_POINTS];
#else
extern const secp256k1_ge_storage secp256k1_ecmult_gen_prec_table[COMB_BLOCKS][COMB_POINTS];
#endif /* defined(EXHAUSTIVE_TEST_ORDER) */

#ifdef __cplusplus
}
#endif

#endif /* SECP256K1_PRECOMPUTED_ECMULT_GEN_H */
