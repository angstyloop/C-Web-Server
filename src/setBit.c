#include "setBit.h"

void setBit(unsigned char* byte, int index){
  byte[index/8] |= 1 << index%8;
}

#ifdef TEST_SET_BIT 
int main(void){
  unsigned char byte=0;
  setBit(&byte, 7);
  assert(byte == 128);
  setBit(&byte, 0);
  assert(byte == 129);
  returnSuccess;
}
#endif
/*
cl /Wall /wd4710 /Fe:test-set-bit /DTEST_SET_BIT setBit.c returnSuccess.c

4710: (snprintf) function not inlined
*/
