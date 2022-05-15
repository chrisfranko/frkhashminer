// frkhash: C/C++ implementation of Frankohash, the Expanse Proof of Work algorithm.
// Copyright 2021-Present Christopher Franko.
// Licensed under the Apache License, Version 2.0.

/// @file
///
/// API design decisions:
///
/// 1. Signed integer type is used whenever the size of the type is not
///    restricted by the Frankohash specification.
///    See http://www.aristeia.com/Papers/C++ReportColumns/sep95.pdf.
///    See https://stackoverflow.com/questions/10168079/why-is-size-t-unsigned/.
///    See https://github.com/Microsoft/GSL/issues/171.

#pragma once

#include <frkhash/frkhash.h>
#include <frkhash/hash_types.hpp>

#include <cstdint>
#include <cstring>
#include <memory>

namespace frkhash
{
constexpr auto revision = FRKHASH_REVISION;

using result = frkhash_result;

/// Constructs a 256-bit hash from an array of bytes.
///
/// @param bytes  A pointer to array of at least 32 bytes.
/// @return       The constructed hash.
inline hash256 hash256_from_bytes(const uint8_t bytes[32]) noexcept
{
    hash256 h;
    std::memcpy(&h, bytes, sizeof(h));
    return h;
}

inline hash256 hash256_from_bytes64(const uint8_t bytes[64]) noexcept
{
    hash256 h;
    std::memcpy(&h, bytes+32, sizeof(h));
    return h;
}

struct search_result
{
    bool solution_found = false;
    uint64_t nonce = 0;
    hash256 final_hash = {};
    hash256 mix_hash = {};

    search_result() noexcept = default;

    search_result(result res, uint64_t n) noexcept
      : solution_found(true), nonce(n), final_hash(res.final_hash), mix_hash(res.mix_hash)
    {}
};

inline result hash(const hash256& header_hash, uint64_t nonce) noexcept
{
    return frkhash_hash(&header_hash, nonce);
}

result hash(const hash256& header_hash, uint64_t nonce) noexcept;

inline bool verify_final_hash(const hash256& header_hash, uint64_t nonce, const hash256& boundary) noexcept
{
    return frkhash_verify_final_hash(&header_hash, nonce, &boundary);
}

inline bool verify(const hash256& header_hash, const hash256& mix_hash, uint64_t nonce, const hash256& boundary) noexcept
{
    return frkhash_verify(&header_hash, &mix_hash, nonce, &boundary);
}

search_result search( const hash256& header_hash, const hash256& boundary, uint64_t start_nonce, size_t iterations) noexcept;

}  // namespace frkhash
