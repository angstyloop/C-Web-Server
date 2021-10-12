#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include "__stdlib.h"
#include "__stdio.h"
#include "countOneBits.h"
#include "countZeroBits.h"
#include "setBit.h"
#include "getBit.h"
#include "unsetBit.h"

#ifdef TEST_BYTE_ARRAY
#include "__assert.h"
#endif

ByteArray* ByteArray_resize(ByteArray* this, size_t newSize);
int isSizeProductBounded(size_t left, size_t right);
size_t boundedSizeProduct(size_t left, size_t right);
int isSizeSumBounded(size_t left, size_t right);
size_t boundedSizeSum(size_t left, size_t right);
int isSizeDifferenceBounded(size_t minuend, size_t subtrahend);
size_t boundedSizeDifference(size_t minuend, size_t subtrahend);
ByteArray* ByteArray_growByFactor(ByteArray* this, size_t factor);
ByteArray* ByteArray_growByDelta(ByteArray* this, size_t delta);
ByteArray* ByteArray_shrinkByFactor(ByteArray* this, size_t factor);
ByteArray* ByteArray_shrinkByDelta(ByteArray* this, size_t delta);
ByteArray* growToAccommodate(ByteArray* this, size_t len);
ByteArray* ByteArray_init(ByteArray* this, size_t len, unsigned char* data);
ByteArray* ByteArray_new(size_t len, unsigned char* data);
ByteArray* ByteArray_append(ByteArray* this, ByteArray* that);
ByteArray* ByteArray_trim(ByteArray* this);
ByteArray* ByteArray_slice(ByteArray* this, size_t start, size_t end);
char* ByteArray_printHeader(ByteArray* this, FILE* out);
char* ByteArray_printSelf(ByteArray* this, FILE* out);
ByteArray* ByteArray_free(ByteArray* this);
ByteArray* ByteArray_countOneBits();
ByteArray* ByteArray_countZeroBits();

typedef unsigned char (*op)(unsigned char, unsigned char) UCharBinOp;

unsigned char UCharBinOp_xor(unsigned char a, unsigned char b);
unsigned char UCharBinOp_and(unsigned char a, unsigned char b);
unsigned char UCharBinOp_or(unsigned char a, unsigned char b);
void ByteArray_checkLengthsMatch(ByteArray* this, ByteArray* that, char* callerName);
ByteArray* ByteArray_applyBinOp(ByteArray* this, ByteArray* that, UCharBinOp op);
ByteArray* ByteArray_binOp(ByteArray* left, ByteArray* right, UCharBinOp op);
ByteArray* ByteArray_xor(ByteArray* this, ByteArray* that);
ByteArray* ByteArray_and(ByteArray* this, ByteArray* that);
ByteArray* ByteArray_or(ByteArray* this, ByteArray* that);
ByteArray* ByteArray_applyXor(ByteArray* this, ByteArray* that);
ByteArray* ByteArray_applyAnd(ByteArray* this, ByteArray* that);
ByteArray* ByteArray_applyOr(ByteArray* this, ByteArray* that);
int ByteArray_getBit(ByteArray* this, size_t index);
ByteArray* ByteArray_setBit(ByteArray* this, size_t index);
ByteArray* ByteArray_unsetBit(ByteArray* this, size_t index);

#endif
