// frkhash: C/C++ implementation of Frankohash, the Expanse Proof of Work algorithm.
// Copyright 2021-Present Christopher Franko.
// Licensed under the Apache License, Version 2.0.

#include "frkhash-internal.hpp"

#include "../support/attributes.h"
#include "bit_manipulation.h"
#include "endianness.hpp"
#include "primes.h"
#include <frkhash/keccak.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <limits>

namespace frkhash
{

namespace
{

inline hash512 hash_seed(const hash256& header_hash, uint64_t nonce) noexcept{
    nonce = le::uint64(nonce);
    uint8_t init_data[sizeof(header_hash) + sizeof(nonce)];
    std::memcpy(&init_data[0], &header_hash, sizeof(header_hash));
    std::memcpy(&init_data[sizeof(header_hash)], &nonce, sizeof(nonce));

    return keccak512(init_data, sizeof(init_data));
}

inline hash256 hash_final(const hash512& seed){
    uint8_t final_data[sizeof(seed)];
    std::memcpy(&final_data[0], &seed, sizeof(seed));
    return keccak256(final_data, sizeof(final_data));
}
}  // namespace


// result hash(const hash256& header_hash, uint64_t nonce) noexcept{
//     const hash512 seed = hash_seed(header_hash, nonce);
//     const hash256 mix_hash = hash256_from_bytes64(seed.bytes);
//     return {hash_final(seed), mix_hash};
// }


search_result search(const hash256& header_hash, const hash256& boundary, uint64_t start_nonce, size_t iterations) noexcept{
    const uint64_t end_nonce = start_nonce + iterations;
    for (uint64_t nonce = start_nonce; nonce < end_nonce; ++nonce)
    {
        result r = frkhash_hash(&header_hash, nonce);
        if (is_less_or_equal(r.final_hash, boundary))
            return {r, nonce};
    }
    return {};
}
}  // namespace frkhash

using namespace frkhash;

extern "C" {

frkhash_result frkhash_hash(const hash256* header_hash, uint64_t nonce) noexcept
{
    const hash512 seed = hash_seed(*header_hash, nonce);
    const hash256 mix_hash = hash256_from_bytes64(seed.bytes);
    return {hash_final(seed), mix_hash};
}

bool frkhash_verify_final_hash(const hash256* header_hash, uint64_t nonce,
    const hash256* boundary) noexcept
{
    const hash512 seed = hash_seed(*header_hash, nonce);
    return is_less_or_equal(hash_final(seed), *boundary);
}

bool frkhash_verify(const hash256* header_hash,
    const hash256* mix_hash, uint64_t nonce, const hash256* boundary) noexcept
{
    const hash512 seed = hash_seed(*header_hash, nonce);
    if (!is_less_or_equal(hash_final(seed), *boundary))
        return false;

    const hash256 expected_mix_hash = hash256_from_bytes64(seed.bytes);
    return is_equal(expected_mix_hash, *mix_hash);
}

}  // extern "C"
