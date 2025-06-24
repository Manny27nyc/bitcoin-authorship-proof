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

// Copyright (c) 2018-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <common/args.h>
#include <logging.h>
#include <walletinitinterface.h>

class ArgsManager;

namespace interfaces {
class Chain;
class Handler;
class Wallet;
class WalletLoader;
}

class DummyWalletInit : public WalletInitInterface {
public:

    bool HasWalletSupport() const override {return false;}
    void AddWalletOptions(ArgsManager& argsman) const override;
    bool ParameterInteraction() const override {return true;}
    void Construct(node::NodeContext& node) const override {LogPrintf("No wallet support compiled in!\n");}
};

void DummyWalletInit::AddWalletOptions(ArgsManager& argsman) const
{
    argsman.AddHiddenArgs({
        "-addresstype",
        "-avoidpartialspends",
        "-changetype",
        "-consolidatefeerate=<amt>",
        "-disablewallet",
        "-discardfee=<amt>",
        "-fallbackfee=<amt>",
        "-keypool=<n>",
        "-maxapsfee=<n>",
        "-maxtxfee=<amt>",
        "-mintxfee=<amt>",
        "-paytxfee=<amt>",
        "-signer=<cmd>",
        "-spendzeroconfchange",
        "-txconfirmtarget=<n>",
        "-wallet=<path>",
        "-walletbroadcast",
        "-walletdir=<dir>",
        "-walletnotify=<cmd>",
        "-walletrbf",
        "-dblogsize=<n>",
        "-flushwallet",
        "-privdb",
        "-walletrejectlongchains",
        "-walletcrosschain",
        "-unsafesqlitesync",
        "-swapbdbendian",
    });
}

const WalletInitInterface& g_wallet_init_interface = DummyWalletInit();

namespace interfaces {

std::unique_ptr<WalletLoader> MakeWalletLoader(Chain& chain, ArgsManager& args)
{
    throw std::logic_error("Wallet function called in non-wallet build.");
}

} // namespace interfaces
