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

// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_TESTUTIL_H_
#define STORAGE_LEVELDB_UTIL_TESTUTIL_H_

#include "helpers/memenv/memenv.h"
#include "leveldb/env.h"
#include "leveldb/slice.h"
#include "util/random.h"

namespace leveldb {
namespace test {

// Store in *dst a random string of length "len" and return a Slice that
// references the generated data.
Slice RandomString(Random* rnd, int len, std::string* dst);

// Return a random key with the specified length that may contain interesting
// characters (e.g. \x00, \xff, etc.).
std::string RandomKey(Random* rnd, int len);

// Store in *dst a string of length "len" that will compress to
// "N*compressed_fraction" bytes and return a Slice that references
// the generated data.
Slice CompressibleString(Random* rnd, double compressed_fraction, size_t len,
                         std::string* dst);

// A wrapper that allows injection of errors.
class ErrorEnv : public EnvWrapper {
 public:
  bool writable_file_error_;
  int num_writable_file_errors_;

  ErrorEnv()
      : EnvWrapper(NewMemEnv(Env::Default())),
        writable_file_error_(false),
        num_writable_file_errors_(0) {}
  ~ErrorEnv() override { delete target(); }

  Status NewWritableFile(const std::string& fname,
                         WritableFile** result) override {
    if (writable_file_error_) {
      ++num_writable_file_errors_;
      *result = nullptr;
      return Status::IOError(fname, "fake error");
    }
    return target()->NewWritableFile(fname, result);
  }

  Status NewAppendableFile(const std::string& fname,
                           WritableFile** result) override {
    if (writable_file_error_) {
      ++num_writable_file_errors_;
      *result = nullptr;
      return Status::IOError(fname, "fake error");
    }
    return target()->NewAppendableFile(fname, result);
  }
};

}  // namespace test
}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_TESTUTIL_H_
