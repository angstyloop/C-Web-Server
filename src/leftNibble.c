#include "leftNibble.h"

unsigned char leftNibble(unsigned char x){
  return (x & 240) >> 4; // 240 = (256-1) - (16-1) = 11110000
}

void TEST_leftNibble(unsigned char* bytes, size_t numBytes){
  for(size_t i=0; i<numBytes; ++i){
    const unsigned char b = bytes[i], left = leftNibble(b);
    assert(0 <= left && left < 256);

    printf("byte: ");
    if(b<16) printf("0");
    printf("%x\n", b);

    printf("left nibble: %x\n", left);
  }
}

#ifdef TEST_LEFT_NIBBLE
int main(){
  size_t numBytes = 1;
  unsigned char* bytes = randomBytes(numBytes); 

  TEST_leftNibble(bytes, numBytes);

  free(bytes);
  returnSuccess();
}
#endif

/*
cl /Zi /Wall /wd4255 /wd4668 /wd4710 /Fe:test-left-nibble /D TEST_LEFT_NIBBLE leftNibble.c errorExitFailure.c error.c errorExit.c exitFailure.c returnSuccess.c .\randomBytes.c
*/
