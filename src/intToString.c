#include "intToString.h"

// uses math
char* intToStringV2(long i){
  char* p=0;
  char z[11]={0};
  p=z
  if(0>i){
    *p++ = '-';
    i *= -1;
  }
  while(i){
    *p++ = '0' + i%10; 
    i/=10;
  } while(i/=10);
  size_t size = p-z/*minus sign or digits*/ + 1/*null byte*/;
  p = calloc(size, 1);
  memcpy(p, z, size);
  return p;
}

// Uses snprintf
char* intToString(long z){
  char* s = calloc(11, 1);
  sprintf(s, 10, "%ld", z);
  size_t n = strlen(s);
  if(n==10) return s;
  char* t = realloc(s, n+1);
  if(!t){
    free(s);
    perrorExit("realloc");
  }
  return t;
}

#ifdef TEST_INT_TO_STRING_H
#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

int main(void){
  char* s=0;

  // Create test file name.

  // Create test file.
  FILE* testFile = fopen("", "rw");

  // Write all the results of intToString next to the fprintf results
  for(int z=LONG_MIN; z<LONG_MAX; ++z){
    s = intToString(z);
    fprintf("%ld %s", z, s);
    free(s);
  }

  return_success();
}
#endif
