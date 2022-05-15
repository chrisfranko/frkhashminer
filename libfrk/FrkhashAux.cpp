
/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPLv3 license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the LICENSE file with
 * this file.
 */

#include "FrkhashAux.h"

#include <frkhash/frkhash.hpp>
#include <frkhash/keccak.hpp>

using namespace dev;
using namespace exp;

Result FrkhashAux::eval(h256 const& _headerHash, uint64_t _nonce) noexcept {

  //frkhash::hash512 mix = frkhash::hash_seed(_headerHash, _nonce);
  //frkhash::hash256 result = frkhash::hash_final(mix);
  auto headerHash = frkhash::hash256_from_bytes(_headerHash.data());
  auto result = frkhash::hash(headerHash, _nonce);
  h256 mix{reinterpret_cast<uint8_t*>(result.mix_hash.bytes), h256::ConstructFromPointer};
  h256 final{reinterpret_cast<uint8_t*>(result.final_hash.bytes), h256::ConstructFromPointer};
  return {final, mix};
}
