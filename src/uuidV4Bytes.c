#include "uuidV4Bytes.h"

unsigned char* uuidV4Bytes(){
  unsigned char* bytes = randomBytes(16);

  unsigned char* timeHiAndVersion =  bytes + 48;
  timeHiAndVersion[0]=0;
  timeHiAndVersion[1]=1;
  timeHiAndVersion[2]=0;
  timeHiAndVersion[3]=0;

  unsigned char* clockSeqHiAndReserved = bytes + 64;
  clockSeqHiAndReserved[0]=1;
  clockSeqHiAndReserved[1]=0;

  return bytes;
}

