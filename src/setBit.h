#ifndef SET_BIT_H
#define SET_BIT_H

#include "__stdint.h"

#ifdef TEST_SET_BIT
#include "__stdlib.h"
#include "__stdio.h"
#include "__assert.h"
#include "returnSuccess.h"
#endif

void setBit(unsigned char* byte, uintmax_t index);

#endif
