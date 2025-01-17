/* frankohash: C/C++ implementation of Frankohash, the Expanse Proof of Work algorithm.
 * Copyright 2018-2020 Christopher Franko.
 * Licensed under the Apache License, Version 2.0.
 */

/**
 * @file
 * Implementation of GCC/clang builtins for MSVC compiler.
 */

#pragma once

#ifdef _MSC_VER
#include <intrin.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the number of leading 0-bits in `x`, starting at the most significant bit position.
 * If `x` is 0, the result is undefined.
 */
inline int __builtin_clz(unsigned int x)
{
    unsigned long most_significant_bit;
    _BitScanReverse(&most_significant_bit, x);
    return 31 - (int)most_significant_bit;
}

/**
 * Returns the number of 1-bits in `x`.
 */
inline int __builtin_popcount(unsigned int x)
{
    return (int)__popcnt(x);
}

#ifdef __cplusplus
}
#endif

#endif
