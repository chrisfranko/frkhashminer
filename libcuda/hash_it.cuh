/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPLv3 license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the LICENSE file with
 * this file.
 */

#include "frkhash_cuda_miner_kernel_globals.h"

#include "frkhash_cuda_miner_kernel.h"

#include "cuda_helper.h"

//#include "eth-sha3.h"

#define _PARALLEL_HASH 4

DEV_INLINE bool compute_hash(uint64_t nonce) {
    // sha3_512(header .. nonce)
    uint2 state[12];

    state[4] = vectorize(nonce);

    keccak_f1600_init(state);

    // keccak_256(keccak_512(header..nonce) .. mix);
    //if (cuda_swab64(keccak_f1600_final(state)) > d_target)
    if (cuda_swab64(keccak_f1600_final(state)) > d_target)
        return true;

    return false;
}
//
// DEV_INLINE bool compute_hash(const uint64_t Nonce)
// {
//   uint32_t MixState[32], TmpBuf[24], OutHash[32];
//
//   // Initial hash - append nonce to header PoW hash and
//   // run it through SHA3 - this becomes the initial value
//   // for the mixing state buffer. The init value is used
//   // later for the final hash, and is therefore saved.
//   memcpy(TmpBuf, (uint8_t *)d_header2, 32);
//   memcpy(TmpBuf + 8, &Nonce, 8);
//   sha3_512((uint8_t *)TmpBuf, 64, (uint8_t *)TmpBuf, 40);
//
//   memcpy(MixState, TmpBuf, 64);
//
//   // The other half of the state is filled by simply
//   // duplicating the first half of its initial value.
//   memcpy(MixState, MixState + 32, 32);
//
//   //memcpy(MixHash, TmpBuf + 16, 32UL);
//
//   // Hash the initial hash and the mix hash concatenated
//   // to get the final proof-of-work hash that is our output.
//   sha3_256((uint8_t *)OutHash, 32, (uint8_t *)TmpBuf, 64);
//   if (*OutHash > d_target)
//       return true;
//
//   return false;
// }
