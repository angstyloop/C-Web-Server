#include "getBit.h"

int getBit(unsigned char* byte, size_t index){
  return (byte[index/8] & (1 << index%8)) ? 1 : 0;
}

#ifdef TEST_GET_BIT 
int main(void){
  {
    unsigned char byte=128;
    assert(getBit(&byte, 7) == 1);
    for(int i=0; i<7; ++i) assert(!getBit(&byte, i));
  }

  {
    unsigned char byte=1;
    assert(getBit(&byte, 0) == 1);
    for(int i=7; i>0; --i) assert(!getBit(&byte, i));
  }

  returnSuccess;
}
#endif
/*
cl /Wall /wd4710 /Fe:test-get-bit /DTEST_GET_BIT getBit.c returnSuccess.c

4710: (snprintf) function not inlined
*/
