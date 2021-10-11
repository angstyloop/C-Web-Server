#include "Uuid.h"

Uuid* Uuid_create(void){
  return calloc(1, sizeof(Uuid));
}

Uuid* Uuid_init(Uuid* this, unsigned char* bytes){
  size_t ofs=0, sz=0;

  sz = UUID_TIME_LOW_SIZE;
  memcpy(this->timeLow, bytes + (ofs += sz), sz);

  sz = UUID_TIME_MID_SIZE;
  memcpy(this->timeMid, bytes + (ofs += sz), sz);

  sz = UUID_TIME_HIGH_AND_VERSION_SIZE;
  memcpy(this->timeHighAndVersion, bytes + (ofs += sz), sz);

  sz = UUID_CLOCK_SEQ_AND_RESERVED_SIZE;
  memcpy(this->clockSeqAndReserved, bytes + (ofs += sz), sz);

  sz = UUID_CLOCK_SEQ_LOW_SIZE;
  memcpy(this->clockSeqLow, bytes + (ofs += sz), sz);

  sz = UUID_NODE_SIZE;
  memcpy(this->node, bytes + (ofs += sz), sz);

  return this;
}

Uuid* Uuid_new(unsigned char* bytes){
  return Uuid_init(Uuid_create(), bytes);
}

Uuid* Uuid_newV4(void){
  unsigned char* bytes = uuidV4Bytes();
  free(bytes);
  Uuid* this = Uuid_new(bytes);
  return this;
}

Uuid* Uuid_free(Uuid* this){
  free(this);
  return this;
}

char* Uuid_hexString(Uuid* this){
  size_t strSize = UUID_SIZE/*bytes*/ * 2/*hex chars/byte*/ + 1/*null byte*/;
  char* str = calloc(strSize, 1);
  char* tmp=0;
  size_t ofs=0;
  size_t sz=0;

  sz = UUID_TIME_LOW_SIZE;
  memcpy(str += ofs += sz, tmp = hexString(this->timeLow, sz), sz);
  free(tmp);

  sz = UUID_TIME_MID_SIZE;
  memcpy(str += ofs += sz, tmp = hexString(this->timeMid, sz), sz);
  free(tmp);

  sz = UUID_TIME_HIGH_AND_VERSION_SIZE;
  memcpy(str += ofs += sz, tmp = hexString(this->timeHighAndVersion, sz), sz);
  free(tmp);

  sz = UUID_CLOCK_SEQ_AND_RESERVED_SIZE;
  memcpy(str += ofs += sz, tmp = hexString(this->clockSeqAndReserved, sz), sz);
  free(tmp);

  sz = UUID_CLOCK_SEQ_LOW_SIZE;
  memcpy(str += ofs += sz, tmp = hexString(this->clockSeqLow, sz), sz);
  free(tmp);

  sz = UUID_NODE_SIZE;
  memcpy(str += ofs += sz, tmp = hexString(this->node, sz), sz);
  free(tmp);

  return str;
}

Uuid* Uuid_printHexString(Uuid* this){
  char* str = Uuid_hexString(this);
  printf("%s\n", str);
  free(str);
  return this;
}

#ifdef TEST_UUID
int main(void){
  Uuid_free(Uuid_printHexString(Uuid_newV4()));
}
#endif

/*
cls; cl /Zi /Wall /wd4255 /wd4668 /wd4710 /Fe:test-uuid /DTEST_UUID uuid.c randomBytes.c uuidV4Bytes.c hexString.c errorExitFailure.c errorExit.c error.c setBit.c unsetBit.c
*
* 4255: Converting () to (void). Some places use empty function argument lists
* without specifying void, and explicitly ignore this warning. On windows, I
* usually use the void specifier for functions without arguments.
*
* 4668: Replacing with '0' for #if/#elif. This warning is explicitly ignored in
* places where windows version macros are defined, likd <sdkddkver.h>. For me, 
* this warning usually occurs when I include <winioctl.h>.
*
* 4710: a member of the printf family of functions was not inlined by the 
* compiler.
*/
