#ifndef RANDOM_BYTES_H
#define RANDOM_BYTES_H

#include "errorExitFailure.h"
#include "returnSuccess.h"

#ifdef _WIN32 // pre-defined windows macro indicating the compile target is x86,
              // x64, 32-bit ARM, or 64-bit ARM.

// If Windows, use the bcrypt API.

#include "__windows.h"

#include "__bcrypt.h"
#include "__stdint.h"

#else

// Otherwise, assume unix, and try to read numBytes from /dev/urandom

// Don't hog the entropy pool.
#define MIN_ALLOWED_ENTROPY_POOL_SIZE 256
#define MAX_ALLOWED_ENTROPY_BITS_REQUESTED 128

#include "__unistd.h"
#include "__stdlib.h"

#endif

#ifdef TEST_RANDOM_BYTES
#include "leftNibble.h"
#include "rightNibble.h"
#endif


/** @brief Return a requested number of secure randomly-generated bytes.
 *  @param numBytes The desired number of cryptographically secure 
 *  randomly-generated bytes. Does not include the terminating null byte.
 *  @return A pointer to an array of unsigned char that must be freed.
 *
 *  @remark Windows and Unix are handled differently. The Windows 
 *  version uses bcrypt. The Unix version uses /dev/urandom.
 *
 */
unsigned char* randomBytes(size_t numBytes);

#endif
