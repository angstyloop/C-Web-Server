#ifndef GET_DELIM_H
#define GET_DELIM_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef LIMITS_H
#define LIMITS_H
#include <limits.h>
#endif

#include "messageReturnSuccess.h"

#include "ssize_t.h"

#define BUFSIZE_DELTA 128
#define BUFSIZE_MIN 4

/** @brief Read a line of text into a buffer $line. Each line is delimited 
 *  by $delimiter.
 *
 *  It is the responsibility of the caller to check the return value of 
 *  getDelim and free $line if -1 is returned.
 *
 *  @param linePtr A pointer to the buffer that stores the line being read 
 *  from the input file $in. If $linePtr points to 0, a new buffer is 
 *  allocated. If the buffer needs to be grown with realloc, the buffer 
 *  pointed to by $linePtr is replaced with the new re-allocated buffer.
 *
 *  @param sizePtr A pointer to the size of the line buffer. If $sizePtr points
 *  to a value that is less than the minimum buffer size BUFSIZE_MIN, a new
 *  buffer of size BUFSIZE_DELTA (the incremental growth size). The size is 
 *  updated along with *linePtr. 
 *
 *  @param delimiter The delimiting character (as an integer, since that's the
 *  return type of getc).
 *
 *  @param in The input file. Must be open for reading "r", or open for reading 
 *  bytes "rb".
 *
 *  @return The number of bytes read into the buffer, including the newline
 *  character, but not including the terminating null byte. Return -1 if 
 *  error. 
 */
ssize_t getDelim(char** linePtr, size_t* sizePtr, int delimiter, FILE* in);

#endif
