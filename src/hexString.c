#include "hexString.h"

char* hexString(unsigned char* bytes, size_t numBytes){
  char* str = calloc(numBytes+1, 1);
  for(int i=0; i<numBytes; ++i)
    snprintf(str, 1, "%s%x", bytes[i] < 16 : "0" : "", bytes[i]);
  return str;
}
