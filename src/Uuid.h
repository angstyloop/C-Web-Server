#ifndef UUID_H
#define UUID_H

#ifdef TEST_UUID
#include "__assert.h"
#include "__stdio.h"
#endif

#include "__stdlib.h"
#include "__string.h"

#include "uuidV4Bytes.h"
#include "hexString.h"
#include "randomBytes.h"

// See RFC 4122 Section 3

// Sizes in bytes for each of the uuid fields 
// (total 16 bytes or 128 bits).
#define UUID_TIME_LOW_SIZE 4
#define UUID_TIME_MID_SIZE 2
#define UUID_TIME_HIGH_AND_VERSION_SIZE 2
#define UUID_CLOCK_SEQ_AND_RESERVED_SIZE 1
#define UUID_CLOCK_SEQ_LOW_SIZE 1
#define UUID_NODE_SIZE 6
#define UUID_SIZE 16

typedef struct Uuid Uuid;
struct Uuid{
  unsigned char timeLow[UUID_TIME_LOW_SIZE];
  unsigned char timeMid[UUID_TIME_MID_SIZE];
  unsigned char timeHighAndVersion[UUID_TIME_HIGH_AND_VERSION_SIZE];
  unsigned char clockSeqAndReserved[UUID_CLOCK_SEQ_AND_RESERVED_SIZE];
  unsigned char clockSeqLow[UUID_CLOCK_SEQ_LOW_SIZE];
  unsigned char node[UUID_NODE_SIZE];
};

Uuid* Uuid_create(void);
Uuid* Uuid_init(Uuid* this, unsigned char* bytes);
Uuid* Uuid_new(unsigned char* bytes);
Uuid* Uuid_newV4(void);
Uuid* Uuid_free(Uuid* this);
char* Uuid_hexString(Uuid* this);

#endif
