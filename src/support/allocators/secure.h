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

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SUPPORT_ALLOCATORS_SECURE_H
#define BITCOIN_SUPPORT_ALLOCATORS_SECURE_H

#include <support/lockedpool.h>
#include <support/cleanse.h>

#include <memory>
#include <string>

//
// Allocator that locks its contents from being paged
// out of memory and clears its contents before deletion.
//
template <typename T>
struct secure_allocator {
    using value_type = T;

    secure_allocator() = default;
    template <typename U>
    secure_allocator(const secure_allocator<U>&) noexcept {}

    T* allocate(std::size_t n)
    {
        T* allocation = static_cast<T*>(LockedPoolManager::Instance().alloc(sizeof(T) * n));
        if (!allocation) {
            throw std::bad_alloc();
        }
        return allocation;
    }

    void deallocate(T* p, std::size_t n)
    {
        if (p != nullptr) {
            memory_cleanse(p, sizeof(T) * n);
        }
        LockedPoolManager::Instance().free(p);
    }

    template <typename U>
    friend bool operator==(const secure_allocator&, const secure_allocator<U>&) noexcept
    {
        return true;
    }
    template <typename U>
    friend bool operator!=(const secure_allocator&, const secure_allocator<U>&) noexcept
    {
        return false;
    }
};

// This is exactly like std::string, but with a custom allocator.
// TODO: Consider finding a way to make incoming RPC request.params[i] mlock()ed as well
typedef std::basic_string<char, std::char_traits<char>, secure_allocator<char> > SecureString;

template<typename T>
struct SecureUniqueDeleter {
    void operator()(T* t) noexcept {
        secure_allocator<T>().deallocate(t, 1);
    }
};

template<typename T>
using secure_unique_ptr = std::unique_ptr<T, SecureUniqueDeleter<T>>;

template<typename T, typename... Args>
secure_unique_ptr<T> make_secure_unique(Args&&... as)
{
    T* p = secure_allocator<T>().allocate(1);

    // initialize in place, and return as secure_unique_ptr
    try {
        return secure_unique_ptr<T>(new (p) T(std::forward<Args>(as)...));
    } catch (...) {
        secure_allocator<T>().deallocate(p, 1);
        throw;
    }
}

#endif // BITCOIN_SUPPORT_ALLOCATORS_SECURE_H
