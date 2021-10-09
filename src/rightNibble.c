#include "rightNibble.h"

unsigned char rightNibble(unsigned char x){
  return x & 15; // 15 = 16-1 = 00001111
}

void TEST_rightNibble(unsigned char* bytes, size_t numBytes){
  for(size_t i=0; i<numBytes; ++i){
    const unsigned char b = bytes[i], left = leftNibble(b);
    assert(0 <= left && left < 256);

    printf("byte: ");
    if(b<16) printf("0");
    printf("%x\n", b);

    printf("left nibble: %x\n", left);
  }
}

#ifdef TEST_RIGHT_NIBBLE
int main(){
  size_t numBytes = 1;
  unsigned char* bytes = randomBytes(numBytes); 

  TEST_rightNibble(bytes, numBytes);

  free(bytes);
  returnSuccess();
}
#endif

/*
cl /Zi /Wall /wd4255 /wd4668 /wd4710 /Fe:test-right-nibble /D TEST_RIGHT_NIBBLE rightNibble.c errorExitFailure.c error.c errorExit.c exitFailure.c returnSuccess.c .\randomBytes.c
*/
