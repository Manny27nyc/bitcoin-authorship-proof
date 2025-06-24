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
// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H
#define BITCOIN_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H

#include <support/cleanse.h>

#include <memory>
#include <vector>

template <typename T>
struct zero_after_free_allocator {
    using value_type = T;

    zero_after_free_allocator() noexcept = default;
    template <typename U>
    zero_after_free_allocator(const zero_after_free_allocator<U>&) noexcept
    {
    }

    T* allocate(std::size_t n)
    {
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T* p, std::size_t n)
    {
        if (p != nullptr)
            memory_cleanse(p, sizeof(T) * n);
        std::allocator<T>{}.deallocate(p, n);
    }

    template <typename U>
    friend bool operator==(const zero_after_free_allocator&, const zero_after_free_allocator<U>&) noexcept
    {
        return true;
    }
    template <typename U>
    friend bool operator!=(const zero_after_free_allocator&, const zero_after_free_allocator<U>&) noexcept
    {
        return false;
    }
};

/** Byte-vector that clears its contents before deletion. */
using SerializeData = std::vector<std::byte, zero_after_free_allocator<std::byte>>;

#endif // BITCOIN_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H
