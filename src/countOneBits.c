#include "countOneBits.h"

uintmax_t countOneBits(size_t size, unsigned char* bytes){
  uintmax_t sum=0;
  const uintmax_t numBits = (uintmax_t)size*8;
  for(uintmax_t i=0; i<numBits; ++i) if(getBit(bytes, i)) ++sum;
  return sum;
}

#ifdef TEST_COUNT_ONE_BITS
int main(void){
  unsigned char bytes[2] = {1, 1};
  assert(countOneBits(2, bytes) == 2);
  returnSuccess;
}
#endif

/*
cl /Wall /Fe:test-count-one-bits /DTEST_COUNT_ONE_BITS countOneBits.c getBit.c
*/
