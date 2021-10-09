#include "getDelim.h"

ssize_t getDelim(
  char** linePtr,
  size_t* sizePtr,
  int delimiter,
  FILE* in)
{
  // Check args
  if(!(linePtr && sizePtr && in)){
    fprintf(stderr, "(getDelim) One or more invalid null arguments.\n");
    return -1;
  }
  // Allocate a new buffer if a $linePtr or $sizeptr point to 0.
  if(!*linePtr || *sizePtr < BUFSIZE_MIN){
    char* temp = realloc(*linePtr, BUFSIZE_DELTA);
    if(!temp){
      perror("realloc");
      return -1;
    }
    *linePtr = temp;
    *sizePtr = BUFSIZE_DELTA;
  }
  // Read the input file $in byte-by-byte, until $delimiter is encountered.
  ssize_t numBytes=0;
  char* pos = *linePtr;
  int c=0;
  while(EOF != (c = getc(in))){
    // If incrementing $numBytes again wouldn't leave room for a terminating
    // null byte in size_t-sized object, this line is too large to process.
    // On a 32-bit machine, this would be a 4GB line, which is obscene. On a 
    // 64-bit machine, it would be much larger. There should be a special case
    // process for lines like that in the caller.
    if(SIZE_MAX <= ++numBytes){
      fprintf(stderr, "(getDelim) Maximum object size exceeded.\n");
      return -1;
    }
    // If the incremented $numBytes is so large that the buffer doesn't have
    // room for it and a terminating null byte, grow the buffer. Be careful
    // not to compare -1 to a size_t. The -1 will become SIZE_MAX.
    if(numBytes > 0 && (size_t)numBytes >= *sizePtr-1){
      // Re-allocate the buffer to a larger size, growing it.
      char* temp = realloc(*linePtr, *sizePtr + BUFSIZE_DELTA);
      if(!temp){
        perror("realloc");
        return -1;
      }
      // Reset the position $pos in the buffer using the re-allocated buffer.
      pos = (*linePtr = temp) + numBytes-1;
      *sizePtr += BUFSIZE_DELTA;
    }
    // Write the character $c to position $pos in the buffer.
    *pos++ = (char)c;
    // If the character we just wrote was the delimiter, we have our line.
    if(c == delimiter) break;
  }
  // Check for error reading file.
  if(ferror(in) || (feof(in) && !numBytes)){
    fprintf(stderr, "(getDelim) Error reading file.\n");
    return -1;
  }
  // Set the terminating null byte on the buffer.
  *pos=0;
  // Return the number of bytes in the line (not counting the terminating null 
  // byte).
  return numBytes;
}

ssize_t getLine(
  char** linePtr,
  size_t* sizePtr,
  FILE* in)
{
  return getDelim(linePtr, sizePtr, '\n', in);
}

#ifdef TEST_GET_LINE

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef ASSERT_H
#define ASSERT_H
#include <assert.h>
#endif

int main(void){
  // Make a temp file with tmpfile.
  FILE* t = tmpfile();

  // Write "123\n456\n789" to a temp file.
  char* s1 = "123\n456\n789";
  size_t n1 = strlen(s1);
  fwrite(s1, 1, n1, t);

  // Reset the file position indicator, and clear the error indicator.
  rewind(t);

  char* line=0;
  size_t size=0;
  ssize_t z=0;

  // Read the first line of the temp file with getLine.
  z = getLine(&line, &size, t);

  // Assert the first line is "123\n".
  assert(!strcmp("123\n", line));

  // Assert the number returned by getLine is 4.
  assert(4==z);

  // Read the second line of the temp file with getLine.
  z = getLine(&line, &size, t);

  // Assert the second line is "456\n".
  assert(!strcmp("456\n", line));

  // Assert the number returned by getLine is 4.
  assert(4==z);

  // Read the third line of the temp file with getLine.
  getLine(&line, &size, t);

  // Assert the number returned by getLine is 3.
  assert(3==z);

  // Assert the third line is "789". Note the newline is not there, since it
  // wasn't in the original text.
  assert(!strcmp("789", line));

  // Reset the file position and error indicators.
  rewind(t);

  // End the last line with a newline, and then follow it with another line,
  // "000\n". Note the newline is there.

  char* s2 = "\n000\n";
  size_t n2 = strlen(s2);
  fwrite(s2, 1, n2, t);

  // Skip the first three lines.
  for(int i=0; i<3; ++i) getLine(&line, &size, t);

  // Get the fourth line.
  z = getLine(&line, &size, t);

  // Assert the fourth line is equal to "000\n".
  assert(!strcmp("000\n", line));

  // Assert the number returned by getLine is 4.
  assert(4==z);

  // Clean up.
  free(line);
  fclose(t);

  // All done.
  messageReturnSuccess("OK");
}

/* 
cl /Zi /Wall /Fe:test-get-delim /D TEST_GET_LINE getDelim.c
*/

#endif

