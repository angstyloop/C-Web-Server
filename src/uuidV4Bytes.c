#include "uuidV4Bytes.h"

unsigned char* uuidV4Bytes(){
  unsigned char* bytes = randomBytes(16);

  size_t timeHiAndVersion = 6;
  unsetBit(bytes + timeHiAndVersion, 0);
  setBit(bytes + timeHiAndVersion, 1);
  unsetBit(bytes + timeHiAndVersion, 2);
  unsetBit(bytes + timeHiAndVersion, 3);

  size_t clockSeqHiAndReserved = 8;
  setBit(bytes + clockSeqHiAndReserved, 0);
  unsetBit(bytes + clockSeqHiAndReserved, 1);

  return bytes;
}

#ifdef TEST_UUID_V4_BYTES
int main(void){
  unsigned char* bytes = uuidV4Bytes();

  for(int i=0; i<16; ++i) printf("%u", bytes[i]);
  puts("");
  free(bytes);

  returnSuccess;
}
#endif
/*
cl /Wall /wd4255 /wd4668 /wd4710 /Fe:test-uuid-v4-bytes /DTEST_UUID_V4_BYTES uuidV4Bytes.c randomBytes.c setBit.c unsetBit.c errorExitFailure.c errorExit.c error.c exitFailure.c

4710: a member of the printf family of functions was not inlined by the 
compiler.

4255: Converting () to (void). Some places use empty function argument lists
without specifying void, and explicitly ignore this warning. On windows, I
usually use the void specifier for functions without arguments.

4668: Replacing with '0' for #if/#elif. This warning is explicitly ignored in
places where windows version macros are defined, likd <sdkddkver.h>. For me, 
this warning usually occurs when I include <winioctl.h>.

*/
