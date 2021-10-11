#include "countOneBits.h"

size_t countOneBits(size_t size, unsigned char* bytes){
  size_t sum=0;
  for(size_t i=0; i<size*8; ++i) if(getBit(bytes, i)) ++sum;
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
