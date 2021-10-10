#include "uuidV4Bytes.h"

unsigned char* uuidV4Bytes(){
  unsigned char* bytes = randomBytes(16);

  size_t timeHiAndVersion = 48;
  setBit(bytes[timeHiAndVersion], 0, 0);
  setBit(bytes[timeHiAndVersion], 1, 1);
  setBit(bytes[timeHiAndVersion], 2, 0);
  setBit(bytes[timeHiAndVersion], 3, 0);

  size_t clockSeqHiAndReserved = 64;
  setBit(bytes[clockSeqHiAndReserved], 0, 1);
  setBit(bytes[clockSeqHiAndReserved], 1, 0);

  return bytes;
}

