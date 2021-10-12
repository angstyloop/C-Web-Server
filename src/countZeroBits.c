#include "countZeroBits.h"

uintmax_t countZeroBits(size_t size, unsigned char* bytes){
  return (uintmax_t)size * 8 - countOneBits(size, bytes);
}

#ifdef TEST_COUNT_ZERO_BITS
int main(void){
  unsigned char bytes[2] = {1, 1};
  assert(countZeroBits(2, bytes) == 14);
  returnSuccess;
}
#endif

/*
cl /Wall /Fe:test-count-zero-bits /DTEST_COUNT_ZERO_BITS countZeroBits.c getBit.c countOneBits.c
*/
