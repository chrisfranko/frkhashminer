

#include frankohash/lib/frankohash/frkhash.cu;


//the mining functions
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
mine<<<4,4>>>(hash256((char[32])eacc9ee71cf0811530b19d329b7b4ef9cea86af6e93888776b2718a517f7719f), 10000000)
