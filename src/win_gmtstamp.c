/** @file: gmtstamp.c
 *  @author: Sean C. Allen (sean.christian.allen.ii@gmail.com)
 *  @date: 2021-09-28
 */
#include "win_gmtstamp.h"

/** @brief (Windows) Return a timestamp for the current time.
 *  
 *  The issue with time.h is that the methods that initialize
 *  the tm structs do work on static-declared objects, which
 *  are stored in the data segment of a compiled C program. 
 *  This means that functions like gmtime are not thread-safe.
 *
 *  A simple workaround is to use a mutex lock to prevent
 *  other threads from calling gmtime at the same time and 
 *  thus accessing the shared memory at the same time.
 *
 *  @return the timestamp, a string formatted after 
 *  RFC 822/1123/2616.
 *
 */

HANDLE hIOMutex={0};

char* win_gmtstamp(void){
  time_t t = time(0);
  struct tm tm;

  // Only allow one thread to call gmtime at a time.
  WaitForSingleObject(hIOMutex, INFINITE);
  tm = *gmtime(&t);
  ReleaseMutex(hIOMutex);

  // This is an example of a format string that adheres to
  // IEEE RFC 822 (and 1123 and 2616[3.3.1 Full Date]). 
  char* fmt = "%a, %d %m %Y %T GMT";

  // Pick an initial size that's likely to work.
  size_t sz = 32;
  char* stamp, * temp = calloc(sz, 1);

  // If the initial size $sz is too small, double it. Repeat
  // until $z is large enough to fit all the output of
  // strftime.
  while(!(strftime(stamp = temp, sz-1, fmt, &tm))){
    // Be sure to deal with $sz getting too large, and the
    // always possible ENOMEM from realloc.
    if(!(SIZE_MAX/2 > sz && (temp = realloc(stamp, sz)))){
      free(stamp);
      perror("realloc");
      exit(EXIT_FAILURE);
    }
  }

  // All done!
  return stamp;
}

// Test

#ifdef TEST_WIN_GMTSTAMP

#  ifndef STDIO_H
#    define STDIO_H
#    include <stdio.h>
#  endif

int main(void){
  printf("%d\n", _MSC_VER);
  //hIOMutex = CreateMutex(0, 0, 0);
  //char* timestamp = win_gmtstamp();
  //puts(timestamp);
  //free(timestamp);
  return EXIT_SUCCESS;
}

#endif

/*
WIN
cl /Wall /Zi /Fe:test-win-gmtstamp /DTEST_WIN_GMTSTAMP win_gmtstamp.c /wd4668 /wd4255 /wd4820 /wd4710 /wd4996
*/
