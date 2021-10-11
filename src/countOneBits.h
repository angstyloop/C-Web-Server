#ifndef COUNT_ONE_BITS_H
#define COUNT_ONE_BITS_H

#include "getBit.h"

#ifdef TEST_COUNT_ONE_BITS
#include "__assert.h"
#include "returnSuccess.h"
#endif

size_t countOneBits(size_t size, unsigned char* bytes);

#endif
