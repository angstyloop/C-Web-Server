#include "numdigits.h"

int numdigits(int n){
  if(!n){return 1;}
  int i=0;
  for(; n; n/=10){++i;}
  return i; 
}

#ifdef TEST_NUMDIGITS

#  ifndef ASSERT_H
#    define ASSERT_H
#    include <assert.h>
#  endif

int main(void){
  assert(1 == numdigits(1)); // 1
  assert(2 == numdigits(23)); // 2
  assert(3 == numdigits(713)); // 3
  assert(5 == numdigits(71310)); // 5
  assert(10 == numdigits(INT_MAX)); // 10
  return EXIT_SUCCESS;
}

#endif

/*
UNIX
gcc -g -Wall -DTEST_NUMDIGITS -o test-numdigits numdigits.c

WINDOWS
cl /Zi /Wall /Fe:test-numdigits /D TEST_NUMDIGITS numdigits.c
*/
