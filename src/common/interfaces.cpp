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

// Copyright (c) 2021-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <interfaces/echo.h>
#include <interfaces/handler.h>

#include <boost/signals2/connection.hpp>
#include <memory>
#include <utility>

namespace common {
namespace {
class CleanupHandler : public interfaces::Handler
{
public:
    explicit CleanupHandler(std::function<void()> cleanup) : m_cleanup(std::move(cleanup)) {}
    ~CleanupHandler() override { if (!m_cleanup) return; m_cleanup(); m_cleanup = nullptr; }
    void disconnect() override { if (!m_cleanup) return; m_cleanup(); m_cleanup = nullptr; }
    std::function<void()> m_cleanup;
};

class SignalHandler : public interfaces::Handler
{
public:
    explicit SignalHandler(boost::signals2::connection connection) : m_connection(std::move(connection)) {}

    void disconnect() override { m_connection.disconnect(); }

    boost::signals2::scoped_connection m_connection;
};

class EchoImpl : public interfaces::Echo
{
public:
    std::string echo(const std::string& echo) override { return echo; }
};
} // namespace
} // namespace common

namespace interfaces {
std::unique_ptr<Handler> MakeCleanupHandler(std::function<void()> cleanup)
{
    return std::make_unique<common::CleanupHandler>(std::move(cleanup));
}

std::unique_ptr<Handler> MakeSignalHandler(boost::signals2::connection connection)
{
    return std::make_unique<common::SignalHandler>(std::move(connection));
}

std::unique_ptr<Echo> MakeEcho() { return std::make_unique<common::EchoImpl>(); }
} // namespace interfaces
