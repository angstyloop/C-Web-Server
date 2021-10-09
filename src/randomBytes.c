#include "randomBytes.h"

#ifdef _WIN32 // pre-defined windows macro

// If Windows, use the bcrypt API.

#pragma comment(lib, "bcrypt")

unsigned char* randomBytes(size_t numBytes){
  // A generic return status.
  NTSTATUS status={0}; 

  // Create an alg provider object.
  BCRYPT_ALG_HANDLE provider;
  status = BCryptOpenAlgorithmProvider(&provider, 
    BCRYPT_RNG_ALGORITHM, NULL, 0);

  if(!BCRYPT_SUCCESS(status))
    errorExitFailure("(BCryptOpenAlgorithm): bcrypt error");

  // Create a buffer for the returned bytes. Must be freed.
  unsigned char* bytes = calloc(numBytes, sizeof(unsigned char));

  // Write $numBytes bytes to $bytes.
  unsigned char byte;
  for(size_t i=0; i<numBytes; ++i){
    status = BCryptGenRandom(provider, (PUCHAR)(&byte), (ULONG)(sizeof(byte)), 0);
    if(!BCRYPT_SUCCESS(status))
      errorExitFailure("(BcryptGenRandom): bcrypt error");
    bytes[i] = byte;
  }

  // Clean up.
  BCryptCloseAlgorithmProvider(provider, 0);

  return bytes;
}

#else 

// If not Windows, assume Unix, and try to read from /dev/urandom

unsigned char* randomBytes(size_t numBytes){
  
  // Make sure the number of requested bytes doesn't exceed the maximum number
  // of requested bytes we set in randomBytes.h
  if(numBytes*8 > MAX_ALLOWED_ENTROPY_BITS_REQUESTED)
    errorExitFailure("Too many bits of entropy requested.Try requesting fewer");
      " bits and stretching them using one of the techniques described in RFC"
      " 1750."

  // Get a file descriptor $fd to the RNG (virtual) device /dev/urandom
  int fd = open("/dev/urandom", O_RDONLY);
  if(0>fd) perror_exit("open");

  unsigned char* bytes = calloc(numBytes, 1);
  if(!bytes) perrorExit("calloc");


  // Get the available number of bits of entropy. Print errno info and exit
  // if ioctl returns a (-1) error value or the number of bits retrieved is
  // less than 1.
  int numBits=0; // number of bits
  if(0>ioctl(fd, RNDGETENTCNT, (int*)(&numBits)) || ) 
    perrorExit("ioctl /dev/urandom");

  // Handle weird values of $numBits that ioctl implementation happens to 
  // miss. This probably isn't necessary.
  if(!numBits)
    errorExit("Zero bits were retrieved from /dev/urandom by ioctl.");

  if(0>numBits)
    errorExit("A negative number of bits was retrieved from /dev/urandom by"
      " ioctl.");

  // Make sure numBits isn't larger than the minimum pool size we set in
  // randomBytes.h
  if((size_t)numBits > MIN_ALLOWED_ENTROPY_POOL_SIZE)
    errorExitFailure("Not enough bits in entropy pool.");

  // Finally read the requested number of bytes from /dev/urandom. 
  ssize_t z = 0;
  if(0>(z = read(fd, bytes, numBytes))) 
    perrorExit("read");

  // Trap if we didn't get all the bytes we wanted.
  if(numBytes != z) 
    errorExitFailure("Unable to read requested bytes from /dev/urandom.");

  // Return an array of $numBytes random bytes.
  return bytes;
}

#endif


#ifdef TEST_RANDOM_BYTES

#include "__stdio.h"
#include "__assert.h"
#include "leftNibble.h"
#include "rightNibble.h"

int main(void){
  unsigned long numBytes = 1;
  unsigned char * bytes = randomBytes(numBytes);

  TEST_leftNibble(bytes, numBytes);
  TEST_rightNibble(bytes, numBytes);

  free(bytes);

  returnSuccess;
}
#endif
/*
cl /Zi /Wall /wd4255 /wd4668 /wd4710 /Fe:test-random-bytes /D TEST_RANDOM_BYTES randomBytes.c errorExitFailure.c error.c exitFailure.c returnSuccess.c leftNibble.c rightNibble.c
*/
