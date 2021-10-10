#include "hexString.h"

/** @brief Interpret a byte array $bytes of size $numBytes (including null byte)
 *  as a hex string. Single-digit hex characters that fit into four or fewer 
 *  bits (0-F) are preceded by a zero, so that every hex character 
 *  representation in the hex string is really a substring of length 2.
 */
char* hexString(unsigned char* bytes, size_t numBytes){
  char* str = calloc(2*numBytes+1, 1);
  for(size_t i=0, j=0; i<numBytes; ++i, j+=2)
    // Safe because a single unsigned char is at most two digits.
    snprintf(str+j, 3, "%.2x", bytes[i]);
  return str;
}

int main(void){
  size_t numBytes = 4;
  unsigned char* bytes = calloc(numBytes, 1);
  bytes[0] = 0;
  bytes[1] = 1;
  bytes[2] = 2;
  bytes[3] = 3;
  char* hexStr = hexString(bytes, numBytes);
  puts(hexStr);
  free(hexStr);
  free(bytes);
  return 0;
}

/*
cl /Wall /wd4710 /Fe:test-hex-string hexString.c

4710: failure to inline (snprintf)
*/
