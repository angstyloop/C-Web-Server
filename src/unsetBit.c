#include "unsetBit.h"

void unsetBit(unsigned char* byte, uintmax_t index){
  byte[index/8] &= ~(1 << index%8);
}

#ifdef TEST_UNSET_BIT 
int main(void){

  {
    unsigned char byte=1;
    unsetBit(&byte, 0);
    assert(!byte);
  }

  {
    unsigned char byte=0;
    unsetBit(&byte, 0);
    assert(!byte);
  }

  returnSuccess;
}
#endif
/*
cl /Wall /wd4710 /Fe:test-unset-bit /DTEST_UNSET_BIT unsetBit.c returnSuccess.c

4710: (snprintf) function not inlined
*/
