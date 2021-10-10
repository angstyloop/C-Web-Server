#ifndef HEX_STRING_H
#define HEX_STRING_H

#include "__stdlib.h" 
#include "__stdio.h"

/** @brief Return a null-terminated string of hex characters representing
 *  an array of bytes $bytes of length $numBytes.
 */
char* hexString(unsigned char* bytes, size_t numBytes);

#endif
