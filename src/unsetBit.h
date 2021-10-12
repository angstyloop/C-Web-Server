#ifndef UNSET_BIT_H

#include "__stdint.h"

#ifdef TEST_UNSET_BIT
#include "assert.h"
#include "returnSuccess.h"
#endif

void unsetBit(unsigned char* byte, uintmax_t index);

#endif
