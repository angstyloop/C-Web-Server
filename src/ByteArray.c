#include "ByteArray.h"

ByteArray* ByteArray_create(size_t size){
  ByteArray* this=0;

  if(!(this = calloc(1, sizeof(ByteArray)))) 
    perrorExit("ByteArray_create calloc this");

  if(!(this->data = calloc(size, 1))){
    free(this);
    perrorExit("ByteArray_create calloc this->size");
  }
  this->size=size;
  return this;
}

ByteArray* ByteArray_zero(ByteArray* this, size_t newSize){}

// resize won't do unneccessary work

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
  if(newSize < this->len){
    this->len = newSize;
  }
  return this;
}
// Technical points:
//
// - We need to check bounds before multiplying two size_t because size_t use
//   arithmetic is modular (like all unsigned integer arithmetic).
//
// - Integer division is floor division, so we need the parity of SIZE_MAX
//   in order to properly check bounds.
//
// - SIZE_MAX could be odd I guess. Consider the cases separately so we can 
//   actually allocate objects of size SIZE_MAX, but no greater. 

int isSizeProductBounded(size_t left, size_t right){
  if(!right) return 1;
  // it's a little pedantice, but good to check if size_max is even
  return left <= SIZE_MAX/right + (!(SIZE_MAX%2)?0:1); 
}

size_t boundedSizeProduct(size_t left, size_t right){
  return isSizeProductBounded(left, right) ? left * right : SIZE_MAX;
}

// Similar considerations are made for a sum of size_t, except the parity 
// doesn't matter.

int isSizeSumBounded(size_t left, size_t right){
  return left <= SIZE_MAX - right;
}

size_t boundedSizeSum(size_t left, size_t right){
  return isSizeSumBounded(left, right) ? left + right : SIZE_MAX;
}

// Similar considerations are made for a difference of size_t, except the
// parity doesn't matter, and now the bound is a lower bound 0.

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

ByteArray* ByteArray_growToAtLeast(ByteArray* this, size_t len){
  if(!this || !this->size){
    return ByteArray_create(len);
  }
  if(this->size >= len){
    return this;
  }
  if(!len || len == SIZE_MAX){
    return ByteArray_resize(this, len);
  }
  // Exponentially increment $size by a pre-defined growth factor until it
  // exceeds $len. 
  size_t size = this->size;
  do{
    size = boundedSizeProduct(size, GROW_FACTOR);
    //printf("size: %zu\n", size);
  }while(size < len);
  return ByteArray_resize(this, size);
}

ByteArray* ByteArray_init(ByteArray* this, size_t len, unsigned char* data){
  ByteArray_growToAtLeast(this, len);
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
  ByteArray_growToAtLeast(this, that->len);
  memcpy(this->data + this->len, that->data, that->len);
  this->len += that->len;
  return this;
}

// Trim by dividing as many times by SHRINK_FACTOR as possible without going 
// below $len.
//
// E.g. If $SHRINK_FACTOR is 2, $size is 16, and $len is 3, trimming will
// resize to 4. If instead $SHRINK_FACTOR is 3, then trimming will resize
// to 5 instead.
//

ByteArray* ByteArray_trim(ByteArray* this){
  size_t size = this->size, temp = size / SHRINK_FACTOR;
  while(temp >= this->len){
    size = temp;
    temp /= SHRINK_FACTOR;
  }
  this->size = size;
  return ByteArray_resize(this, size);
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
typedef unsigned char (*UCharBinOp)(unsigned char, unsigned char);

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

  puts("TEST");

  puts("");
  puts("create");
  ByteArray* ba = ByteArray_create(1);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");

  puts("");
  puts("init");
  ByteArray_init(ba, len, data);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");
//XXX
  puts("");
  puts("resize");
  ByteArray_resize(ba, size*=2);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");

  puts("");
  puts("free");
  ByteArray_free(ba);
  puts("");

  puts("");
  puts("create");
  ba = ByteArray_create(size);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");

  puts("");
  puts("init using $data with $len > $ba->size, to force a resize"); 
  unsigned char* temp = realloc(data, len*=10);
  if(!temp) perrorExit("realloc");
  data=temp;
  memset(data, 1, len);
  ByteArray_init(ba, len, data);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");

  puts("");
  puts("growByFactor");
  ByteArray_growByFactor(ba, 2);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");

  puts("");
  puts("shrinkByFactor");
  ByteArray_shrinkByFactor(ba, 2);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");

  puts("");
  puts("growByDelta");
  ByteArray_growByDelta(ba, size = ba->size);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");

  puts("");
  puts("shrinkByDelta");
  ByteArray_shrinkByDelta(ba, size);
  puts("print");
  ByteArray_print(ba, stdout);
  puts("");

  puts("");
  puts("free");
  ByteArray_free(ba);

  puts("");
  puts("OK");
  returnSuccess;
}
#endif

/*
cl /Wall /wd4706 /wd4710 /Fe:test-byte-array /DTEST_BYTE_ARRAY ByteArray.c getBit.c setBit.c unsetBit.c countOneBits.c countZeroBits.c exitFailure.c perrorExit.c

4710: (fprintf) member of printf family of functions not inlined
4706: assignment within conditional expression
*/
