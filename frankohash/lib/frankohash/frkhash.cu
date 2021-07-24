#include frankohash.cpp;

int mine(const hash256* header_hash, int diff) {
   int nonce = 0;
   while (true) {
      //add condition for diff
      if(add_value<<<1,1>>>(frankohash_hash(header_hash, nonce)::final_hash::str) <= diff) {
         return nonce;
         break;
      }
   }
}
