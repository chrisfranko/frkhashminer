#include frankohash.cpp;

int mine(const hash256* header_hash) {
   int nonce = 0;
   while (true) {
      //add condition for diff
      if(frankohash_hash(header_hash, nonce)) {
         return nonce;
         break;
      }
   }
}
