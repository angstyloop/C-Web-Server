#include "uuidV4HexString.h"

char* uuidV4HexString(){
  unsigned char* bytes =  uuidV4Bytes();
  char* str = hexString(bytes);
  free(bytes);
  return str;
}

