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

// Copyright (c) 2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <filesystem>
#include <fstream>
#include <future>
#include <init.capnp.h>
#include <init.capnp.proxy.h>
#include <iostream>
#include <mp/proxy-io.h>
#include <mp/util.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

namespace fs = std::filesystem;

static auto Spawn(mp::EventLoop& loop, const std::string& process_argv0, const std::string& new_exe_name)
{
    int pid;
    const int fd = mp::SpawnProcess(pid, [&](int fd) -> std::vector<std::string> {
        fs::path path = process_argv0;
        path.remove_filename();
        path.append(new_exe_name);
        return {path.string(), std::to_string(fd)};
    });
    return std::make_tuple(mp::ConnectStream<InitInterface>(loop, fd), pid);
}

static void LogPrint(bool raise, const std::string& message)
{
    if (raise) throw std::runtime_error(message);
    std::ofstream("debug.log", std::ios_base::app) << message << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 1) {
        std::cout << "Usage: mpexample\n";
        return 1;
    }

    std::promise<mp::EventLoop*> promise;
    std::thread loop_thread([&] {
        mp::EventLoop loop("mpexample", LogPrint);
        promise.set_value(&loop);
        loop.loop();
    });
    mp::EventLoop* loop = promise.get_future().get();

    auto [printer_init, printer_pid] = Spawn(*loop, argv[0], "mpprinter");
    auto [calc_init, calc_pid] = Spawn(*loop, argv[0], "mpcalculator");
    auto calc = calc_init->makeCalculator(printer_init->makePrinter());
    while (true) {
        std::string eqn;
        std::cout << "Enter the equation, or \"exit\" to quit: ";
        std::getline(std::cin, eqn);
        if (eqn == "exit") break;
        calc->solveEquation(eqn);
    }
    calc.reset();
    calc_init.reset();
    mp::WaitProcess(calc_pid);
    printer_init.reset();
    mp::WaitProcess(printer_pid);
    loop_thread.join();
    std::cout << "Bye!" << std::endl;
    return 0;
}
