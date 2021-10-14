#include "ByteArray.h"

ByteArray* ByteArray_create(size_t size){
  ByteArray* this=0L;

  if(!(this = calloc(1, sizeof(ByteArray)))) 
    perrorExit("ByteArray_create calloc this");

  if(!(this->data = calloc(size, 1))){
    free(this);
    perrorExit("ByteArray_create calloc this->size");
  }
  this->size=size;
  return this;
}

ByteArray* ByteArray_fill(ByteArray* this, unsigned char byte){
  memset(this->data, (int)byte, this->size);
  return this;
}

ByteArray* ByteArray_zero(ByteArray* this){
  return ByteArray_fill(this, 0);
}

ByteArray* ByteArray_resize(ByteArray* this, size_t newSize){
  if(this->size == newSize){
    return this;
  }
  if(!newSize){
    return ByteArray_free(this);
  }
  unsigned char* temp = realloc(this->data, newSize);
  if(!temp){
    ByteArray_free(this);
    perrorExit("realloc");
  };
  this->data = temp; 
  this->size = newSize;
  if(newSize < this->len){ // if truncating, set the length to the new size
    this->len = newSize;
  } else if(newSize > this->len){ // if expanding, pad end with zeroes
    memset(this->data + this->len, 0, this->size - this->len + 1);
  }
  return this;
}
// Technical points:
//
// - We need to check bounds before multiplying two size_t because size_t use
//   arithmetic is modular (like all unsigned integer arithmetic).
//
// - Integer division is floor division. Inverting floor division two nonzero
//   positive integers n/m = l always gives m possible values for n in the
//   interval [l*m, (l+1)*m).
//
//   So n/m < l does not necessarily imply n < l*m. Instead,
//
//     n/m < l => n < (l+1)*m
//
//   Now suppose n/m > l. Then n >= l*m.
//
//   Thus, to guarantee the product of two size_t n and m is less than or equal
//   to SIZE_MAX (n*m <= SIZE_MAX), it is sufficient to require the quotient 
//   of SIZE_MAX with one of them be strictly greater than the other 
//   (n < SIZE_MAX/m or m < SIZE_MAX/n), avoiding dividing by zero of course.

int isSizeProductBounded(size_t left, size_t right){
  if(!right) return 1;
  return left < SIZE_MAX/right; 
}

size_t boundedSizeProduct(size_t left, size_t right){
  return isSizeProductBounded(left, right) ? left * right : SIZE_MAX;
}

int isSizeSumBounded(size_t left, size_t right){
  return left <= SIZE_MAX - right;
}

size_t boundedSizeSum(size_t left, size_t right){
  return isSizeSumBounded(left, right) ? left + right : SIZE_MAX;
}

int isSizeDifferenceBounded(size_t minuend, size_t subtrahend){
  return subtrahend <= minuend;
}

size_t boundedSizeDifference(size_t minuend, size_t subtrahend){
  return isSizeDifferenceBounded(minuend, subtrahend) ? 
    minuend - subtrahend: 
    0;
}

// All the grow and shrink operations
//
//   - are the identity operation when their 2nd argument is zero. So attempting
//     to grow/shrink by a zero-valued factor/delta will simply return the same 
//     object. 
//
//   - will not grow above SIZE_MAX, and will not shrink below 0. Repeated grows
//     to SIZE_MAX or shrinks to 0 are idempotent - they return the same object.
//     Whenever the size would be less than 0, the size 0 is used. Whenever the
//     size would be greater than SIZE_MAX, then SIZE_MAX is used. Resizing to
//     zero is the same as freeing.

ByteArray* ByteArray_growByFactor(ByteArray* this, size_t factor){
  return (!factor || this->size == SIZE_MAX)? 
    this:
    ByteArray_resize(this, boundedSizeProduct(this->size, factor));
}

ByteArray* ByteArray_growByDelta(ByteArray* this, size_t delta){
  return (!delta || this->size == SIZE_MAX)?
    this:
    ByteArray_resize(this, boundedSizeSum(this->size, delta));
}

// Note that mod division is idempotent with limit at 0, so we don't have to 
// worry about wrapping.

ByteArray* ByteArray_shrinkByFactor(ByteArray* this, size_t factor){
  return (!factor || !this->size)?
    this:
    ByteArray_resize(this, this->size / factor);
}

// We do have to worry about wrapping with modulo subtraction though.
ByteArray* ByteArray_shrinkByDelta(ByteArray* this, size_t delta){
  return (!delta || !this->size)?
    this:
    ByteArray_resize(this, boundedSizeDifference(this->size, delta));
}

#define GROW_FACTOR 2
#define SHRINK_FACTOR GROW_FACTOR

ByteArray* ByteArray_growUntil(ByteArray* this, size_t lowerBound){
  if(!this->size){
    return ByteArray_create(lowerBound);
  }
  if(this->size >= lowerBound){
    return this;
  }
  if(!lowerBound || lowerBound == SIZE_MAX){
    return ByteArray_resize(this, lowerBound);
  }
  // Exponentially increment $size by a pre-defined growth factor until it
  // exceeds $lowerBound. 
  size_t size = this->size;
  do{
    size = boundedSizeProduct(size, GROW_FACTOR);
    //printf("size: %zu\n", size);
  }while(size < lowerBound);
  return ByteArray_resize(this, size);
}

ByteArray* ByteArray_init(ByteArray* this, size_t len, unsigned char* data){
  ByteArray_growUntil(this, len);
  memcpy(this->data, data, len);
  this->len=len;
  return this;
}

ByteArray* ByteArray_new(size_t len, unsigned char* data){
  return ByteArray_init(ByteArray_create(len), len, data);
}

// append $that to $this

ByteArray* ByteArray_append(ByteArray* this, ByteArray* that){
  if(!that && !that->len){
    return this;
  }
  ByteArray_growUntil(this, that->len);
  memcpy(this->data + this->len, that->data, that->len);
  this->len += that->len;
  return this;
}

// Reduce size as much as possible without truncating data, by dividing as many 
// times by SHRINK_FACTOR as possible without going below $len.
// E.g. If $SHRINK_FACTOR is 2, $size is 16, and $len is 3, 16 will be resized
// to 4. If instead $SHRINK_FACTOR is 3, then 16 will be resized to 5.

ByteArray* ByteArray_shrinkUntil(ByteArray* this, size_t lowerBound){
  if(!this->size || lowerBound >= this->len){
    return this;
  }
  size_t size = this->size, temp = size / SHRINK_FACTOR;
  while(temp >= lowerBound){
    size = temp;
    temp /= SHRINK_FACTOR;
  }
  this->size = size;
  return ByteArray_resize(this, size);
}

// Trim size of the buffer to the length of the data.

ByteArray* ByteArray_trim(ByteArray* this){
  return ByteArray_resize(this, this->len);
}

// [start,end)

ByteArray* ByteArray_slice(ByteArray* this, size_t start, size_t end){
  return ByteArray_new(end-start+1, this->data + start);
}

ByteArray* ByteArray_printHeader(ByteArray* this, FILE* out){
  fprintf(out, "%s, %s, %s\n", "len", "size", "data");
  return this;
}

ByteArray* ByteArray_print(ByteArray* this, FILE* out){
  // Print $len
  fprintf(out, "%zu, ", this->len);
  // Print $size and open brace of $data array.
  fprintf(out, "%zu, {", this->size);
  // If $data is not 0, print $data array contents, except last item.
  if(!this->data || !this->len){
    fprintf(out, "}");
    return this;
  }
  for(size_t i=0; i<this->len-1; ++i){
    fprintf(out, "%u, ", this->data[i]);
  }
  // Print last item of $data and close brace of $data array.
  fprintf(out, "%u}\n", this->data[this->len-1]);
  return this;
}

ByteArray* ByteArray_free(ByteArray* this){
  free(this->data);
  this->len = this->size = 0; 
  this->data = 0;
  free(this);
  return this;
}

uintmax_t ByteArray_countOneBits(ByteArray* this){
  return countOneBits(this->len, this->data);
}

uintmax_t ByteArray_countZeroBits(ByteArray* this){
  return countZeroBits(this->len, this->data);
}

// TODO: try removing this
//typedef unsigned char (*UCharBinOp)(unsigned char, unsigned char);

unsigned char UCharBinOp_xor(unsigned char a, unsigned char b){
  return a^b;
}

unsigned char UCharBinOp_and(unsigned char a, unsigned char b){
  return a&b;
}

unsigned char UCharBinOp_or(unsigned char a, unsigned char b){
  return a|b;
}

void ByteArray_checkLengthsMatch(ByteArray* this, ByteArray* that, char* callerName){
  if(this->len != that->len){
    fprintf(stderr, "(ByteArray%s%s) Lengths don't match.", 
      callerName ? "_" : "", callerName ? callerName : "");
    exitFailure();
  }
}

ByteArray* ByteArray_applyBinOp(ByteArray* this, ByteArray* that, UCharBinOp op){
  ByteArray_checkLengthsMatch(this, that, "applyBinOp");
  for(size_t i=0; i<this->len; ++i)
    this->data[i] = op(this->data[i], that->data[i]);
  return this;
}

ByteArray* ByteArray_binOp(ByteArray* left, ByteArray* right, UCharBinOp op){
  ByteArray_checkLengthsMatch(left, right, "applyBinOp");
  ByteArray* out = ByteArray_create(left->len);
  for(size_t i=0; i<left->len; ++i){
    out->data[i] = op(left->data[i], right->data[i]);
  }
  return out;
}

ByteArray* ByteArray_xor(ByteArray* this, ByteArray* that){
  return ByteArray_binOp(this, that, UCharBinOp_xor);
}

ByteArray* ByteArray_and(ByteArray* this, ByteArray* that){
  return ByteArray_binOp(this, that, UCharBinOp_and);
}

ByteArray* ByteArray_or(ByteArray* this, ByteArray* that){
  return ByteArray_binOp(this, that, UCharBinOp_or);
}

ByteArray* ByteArray_applyXor(ByteArray* this, ByteArray* that){
  return ByteArray_applyBinOp(this, that, UCharBinOp_xor);
}

ByteArray* ByteArray_applyAnd(ByteArray* this, ByteArray* that){
  return ByteArray_applyBinOp(this, that, UCharBinOp_and);
}

ByteArray* ByteArray_applyOr(ByteArray* this, ByteArray* that){
  return ByteArray_applyBinOp(this, that, UCharBinOp_or);
}

int ByteArray_getBit(ByteArray* this, size_t index){
  return getBit(this->data, index);
}

ByteArray* ByteArray_setBit(ByteArray* this, uintmax_t index){
  setBit(this->data, index);
  return this;
}

ByteArray* ByteArray_unsetBit(ByteArray* this, uintmax_t index){
  unsetBit(this->data, index);
  return this;
}

#ifdef TEST_BYTE_ARRAY
int main(void){
  size_t len, size; len=size=1;
  unsigned char* data = calloc(1, 1);
  if(!data) perrorExit("calloc");
  data[0]=1;

  puts("TEST\n");

  puts("create");
  ByteArray* ba = ByteArray_create(1);
  puts("\n");

  puts("print");
  ByteArray_print(ba, stdout);
  printf("\n\n");

  puts("init");
  ByteArray_init(ba, len, data);
  ByteArray_print(ba, stdout);
  printf("\n\n");

  puts("resize");
  ByteArray_resize(ba, size*=2);
  ByteArray_print(ba, stdout);
  printf("\n\n");

  puts("free");
  ByteArray_free(ba);
  printf("\n\n");

  puts("create");
  ba = ByteArray_create(size);
  ByteArray_print(ba, stdout);
  puts("\n");

  puts("init using $data with $len > $ba->size, to force a resize"); 
  unsigned char* temp = realloc(data, len*=10);
  if(!temp) perrorExit("realloc");
  data=temp;
  memset(data, 1, len);
  ByteArray_init(ba, len, data);
  ByteArray_print(ba, stdout);
  puts("\n");

  puts("growByFactor");
  ByteArray_growByFactor(ba, 2);
  ByteArray_print(ba, stdout);
  puts("\n");

  puts("shrinkByFactor");
  ByteArray_shrinkByFactor(ba, 2);
  ByteArray_print(ba, stdout);
  puts("\n");

  puts("growByDelta");
  ByteArray_growByDelta(ba, size = ba->size);
  ByteArray_print(ba, stdout);
  puts("\n");

  puts("shrinkByDelta");
  ByteArray_shrinkByDelta(ba, size);
  ByteArray_print(ba, stdout);
  puts("\n");

  puts("growByFactor");
  ByteArray_growByFactor(ba, 2);
  ByteArray_print(ba, stdout);
  puts("\n");

  puts("trim");
  ByteArray_trim(ba);
  ByteArray_print(ba, stdout);
  puts("\n");

  puts("free");
  ByteArray_free(ba);
  puts("\n");

  puts("OK");
  returnSuccess;
}
#endif

/*
cl /Wall /wd4706 /wd4710 /Fe:test-byte-array /DTEST_BYTE_ARRAY ByteArray.c getBit.c setBit.c unsetBit.c countOneBits.c countZeroBits.c exitFailure.c perrorExit.c

4710: (fprintf) member of printf family of functions not inlined
4706: assignment within conditional expression
*/
