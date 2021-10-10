#ifndef UUID_V4_BYTES_H
#define UUID_V4_BYTES_H

#include "randomBytes.h"
#include "unsetBit.h"
#include "setBit.h"

#ifdef TEST_UUID_V4_BYTES_H
#include "returnSuccess.h"
#include "stdio.h"
#endif

/** @brief Return a byte array representation of a Version 4 UKUID as defined
 *  by RFC 4122.
 */
unsigned char* uuidV4Bytes(void);

#endif
