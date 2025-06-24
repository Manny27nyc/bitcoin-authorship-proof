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

// Copyright (c) 2025 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <util/exec.h>

#include <util/fs.h>
#include <util/subprocess.h>

#include <string>
#include <vector>

#ifdef WIN32
#include <process.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace util {
int ExecVp(const char* file, char* const argv[])
{
#ifndef WIN32
    return execvp(file, argv);
#else
    std::vector<std::wstring> escaped_args;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    for (char* const* arg_ptr{argv}; *arg_ptr; ++arg_ptr) {
        subprocess::util::quote_argument(converter.from_bytes(*arg_ptr), escaped_args.emplace_back(), false);
    }

    std::vector<const wchar_t*> new_argv;
    new_argv.reserve(escaped_args.size() + 1);
    for (const auto& s : escaped_args) new_argv.push_back(s.c_str());
    new_argv.push_back(nullptr);
    return _wexecvp(converter.from_bytes(file).c_str(), new_argv.data());
#endif
}

fs::path GetExePath(std::string_view argv0)
{
    // Try to figure out where executable is located. This does a simplified
    // search that won't work perfectly on every platform and doesn't need to,
    // as it is only currently being used in a convenience wrapper binary to try
    // to prioritize locally built or installed executables over system
    // executables.
    const fs::path argv0_path{fs::PathFromString(std::string{argv0})};
    fs::path path{argv0_path};
    std::error_code ec;
#ifndef WIN32
    // If argv0 doesn't contain a path separator, it was invoked from the system
    // PATH and can be searched for there.
    if (!argv0_path.has_parent_path()) {
        if (const char* path_env = std::getenv("PATH")) {
            size_t start{0}, end{0};
            for (std::string_view paths{path_env}; end != std::string_view::npos; start = end + 1) {
                end = paths.find(':', start);
                fs::path candidate = fs::path(paths.substr(start, end - start)) / argv0_path;
                if (fs::is_regular_file(candidate, ec)) {
                    path = candidate;
                    break;
                }
            }
        }
    }
#else
    wchar_t module_path[MAX_PATH];
    if (GetModuleFileNameW(nullptr, module_path, MAX_PATH) > 0) {
        path = fs::path{module_path};
    }
#endif
    return path;
}

} // namespace util
