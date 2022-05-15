/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPLv3 license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the LICENSE file with
 * this file.
 */

#include "frkhash_cuda_miner_kernel.h"

#include "frkhash_cuda_miner_kernel_globals.h"

#include "cuda_helper.h"

#define copy(dst, src, count)                                                                                          \
    for (int i = 0; i != count; ++i) {                                                                                 \
        (dst)[i] = (src)[i];                                                                                           \
    }

#include "keccak.cuh"
//#include "eth-sha3.cuh"
#include "hash_it.cuh"

__global__ void frkhash_search(Search_results* g_output, uint64_t start_nonce) {
    if (g_output->done)
        return;
    uint32_t const gid = blockIdx.x * blockDim.x + threadIdx.x;
    bool r = compute_hash(start_nonce + gid);
    if (threadIdx.x == 0)
        atomicInc((uint32_t*)&g_output->hashCount, 0xffffffff);
    if (r)
        return;
    uint32_t index = atomicInc((uint32_t*)&g_output->solCount, 0xffffffff);
    if (index >= MAX_SEARCH_RESULTS)
        return;
    g_output->gid[index] = gid;
    g_output->done = 1;
}

void run_frkhash_search(uint32_t gridSize, uint32_t blockSize, cudaStream_t stream, Search_results* g_output,
                       uint64_t start_nonce) {
    frkhash_search<<<gridSize, blockSize, 0, stream>>>(g_output, start_nonce);
    CUDA_CALL(cudaGetLastError());
}

/*
__global__ void frkhash_calculate_dag_item(uint32_t start) {
    uint32_t const node_index = start + blockIdx.x * blockDim.x + threadIdx.x;
    if (((node_index >> 1) & (~1)) >= d_dag_size)
        return;
    union {
        hash128_t dag_node;
        uint2 sha3_buf[25];
    };
    copy(dag_node.uint4s, d_light[node_index % d_light_size].uint4s, 4);
    dag_node.words[0] ^= node_index;
    SHA3_512(sha3_buf);

    const int thread_id = threadIdx.x & 3;

    for (uint32_t i = 0; i != FRKHASH_DATASET_PARENTS; ++i) {
        uint32_t parent_index = fnv(node_index ^ i, dag_node.words[i % NODE_WORDS]) % d_light_size;
        for (uint32_t t = 0; t < 4; t++) {
            uint32_t shuffle_index = SHFL(parent_index, t, 4);

            uint4 p4 = d_light[shuffle_index].uint4s[thread_id];
            for (int w = 0; w < 4; w++) {
                uint4 s4 = make_uint4(SHFL(p4.x, w, 4), SHFL(p4.y, w, 4), SHFL(p4.z, w, 4), SHFL(p4.w, w, 4));
                if (t == thread_id) {
                    dag_node.uint4s[w] = fnv4(dag_node.uint4s[w], s4);
                }
            }
        }
    }
    SHA3_512(sha3_buf);
    hash64_t* dag_nodes = (hash64_t*)d_dag;
    copy(dag_nodes[node_index].uint4s, dag_node.uint4s, 4);
}
*/
void set_header(hash32_t _header) { CUDA_CALL(cudaMemcpyToSymbol(d_header, &_header, sizeof(hash32_t))); }
void set_target(uint64_t _target) { CUDA_CALL(cudaMemcpyToSymbol(d_target, &_target, sizeof(uint64_t))); }
