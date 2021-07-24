#include frankohash.cpp;

int add_value(char input[32]) {
   int output = 0;
   for(int i=0; i<=32; i++) {
      output += (int)input[i]
   }
}

int mine(const hash256* header_hash, int diff) {
   int nonce = 0;
   while (true) {
      //add condition for diff
      if(add_value<<<1,1>>>(frankohash_hash(header_hash, nonce)::final_hash::str) <= diff) {
         return nonce;
         break;
      }
      nonce += 1;
   }
}
// fetch the header and mine it
mine<<<4,4>>>()