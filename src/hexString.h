#ifndef HEX_STRING_H
#define HEX_STRING_H

/** @brief Return a null-terminated string of hex characters representing
 *  an array of bytes $bytes of length $numBytes.
 */
char* hexString(unsigned char* bytes, size_t numBytes);

#endif
