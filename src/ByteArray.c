typedef struct ByteArray ByteArray;
struct ByteArray{
  size_t len;
  size_t size;
  unsigned char* data;
};

ByteArray* ByteArray_create(size_t size);
ByteArray* ByteArray_create(size_t size){
  ByteArray* this = calloc(size, sizeof(ByteArray));
  return this;
}

//void ByteArray_checkLengthBounds(ByteArray* this, size_t len);
//void ByteArray_checkLengthBounds(ByteArray* this, size_t len){
// if(len > this->size){
//   fprintf(stderr, "len: %zu, size: %zu\n", len, this->size);
//   ByteArray_free(this);
//   errorExitFailure("(ByteArray_init) Requested length exceeds allocated size.");
// }
//}

// May truncate, in which case length is modified.

ByteArray* ByteArray_resize(ByteArray* this, size_t newSize);
ByteArray* ByteArray_resize(ByteArray* this, size_t newSize){
  if(this->size == newSize){
    return this;
  }
  if(!newSize){
    return ByteArray_free(this);
  }
  char* temp = realloc(this->data, newSize);
  if(!temp){
    ByteArray_free(this);
    perrorExitFailure("realloc");
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

// This macro will be convenient 
#define IS_SIZE_MAX_EVEN (SIZE_MAX % 2 == 0)

int isSizeProductBounded(size_t left, size_t right);
int isSizeProductBounded(size_t left, size_t right){
  if(!right) return 1;
  return left <= SIZE_MAX/right + (IS_SIZE_MAX_EVEN ? 0 : 1);
}

size_t boundedSizeProduct(size_t left, size_t right);
size_t boundedSizeProduct(size_t left, size_t right){
  return isSizeProductBounded(left, right) ? left * right : SIZE_MAX;
}

// Similar considerations are made for a sum of size_t, except the parity 
// doesn't matter.

int isSizeSumBounded(size_t left, size_t right);
int isSizeSumBounded(size_t left, size_t right){
  return left <= SIZE_MAX - right;
}

size_t boundedSizeSum(size_t left, size_t right);
size_t boundedSizeSum(size_t left, size_t right){
  return isSizeSumBounded(left, right) ? left + right : SIZE_MAX;
}

// Similar considerations are made for a difference of size_t, except the
// parity doesn't matter, and now the bound is a lower bound 0.

int isSizeDifferenceBounded(size_t minuend, size_t subtrahend);
int isSizeDifferenceBounded(size_t minuend, size_t subtrahend){
  return subtrahend <= minuend;
}

size_t boundedSizeDifference(size_t minuend, size_t subtrahend);
size_t boundedSizeDifference(size_t minuend, size_t subtrahend){
  return isSizeDifferenceBounded() ? minuend - subtrahend : 0;
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
  return if(!delta || !this->size)?
    this:
    ByteArray_resize(this, boundedSizeDifference(this->size, delta));
}

#define GROW_FACTOR 2
#define SHRINK_FACTOR GROW_FACTOR

ByteArray* growToAccommodate(ByteArray* this, size_t len);
ByteArray* growToAccommodate(ByteArray* this, size_t len){
  if (this->size >= len){
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
  }while(size < len);
  return ByteArray_resize(this, size);
}

ByteArray* ByteArray_init(ByteArray* this, size_t len, unsigned char* data);
ByteArray* ByteArray_init(ByteArray* this, size_t len, unsigned char* data){
  growToAccommodate(ByteArray* this, size_t len);
  memcpy(this->data, data, len);
  return this;
}

ByteArray* ByteArray_new(size_t len, unsigned char* data);
ByteArray* ByteArray_new(size_t len, unsigned char* data){
  return ByteArray_init(ByteArray_create(len), data);
}

// append $that to $this

ByteArray* ByteArray_append(ByteArray* this, ByteArray* that){
  if(!that && !that->len){
    return this;
  }
  this->growToAccomodate(this, that->len);
  memcpy(this->data + this->len, that->data, that->len);
  this->len += that->len;
  return this;
}

ByteArray* ByteArray_trim(ByteArray* this){
  
}

// [start,end)

ByteArray* ByteArray_slice(ByteArray* this, size_t start, size_t end){
  return ByteArray_new(end-start+1, this->data + start);
}

char* ByteArray_printHeader(ByteArray* this, FILE* out);
char* ByteArray_printHeader(ByteArray* this, FILE* out){
  fprintf("%s, %s, %s\n", "len", "size", "data");
  return this;
}

char* ByteArray_printSelf(ByteArray* this, FILE* out);
char* ByteArray_printSelf(ByteArray* this, FILE* out){
  // Print $len
  fprintf(out, "%zu, ", this->len);
  // Print $size and open brace of $data array.
  fprintf(out, "%zu, {", this->size);
  // Print $data array contents, except last item.
  for(int i=0; i<this->len-1; ++i){
    fprintf(out, "%u, ", this->data[i]);
  }
  // Print last item of $data and close brace of $data array.
  fprintf(out, "%u}\n", last);
  return this;
}

ByteArray* ByteArray_free(ByteArray* this);
ByteArray* ByteArray_free(ByteArray* this){
  free(data);
  this->len = this->size = this->data = 0;
  free(this);
}

ByteArray* ByteArray_countOneBits(){
  return countOneBits(len, this->data);
}

ByteArray* ByteArray_countZeroBits(){
  return countZeroBits(len, this->data);
}

typedef unsigned char (*op)(unsigned char, unsigned char) UCharBinOp;

unsigned char UCharBinOp_xor(unsigned char a, unsigned char b);
unsigned char UCharBinOp_xor(unsigned char a, unsigned char b){
  return a^b;
}

unsigned char UCharBinOp_and(unsigned char a, unsigned char b);
unsigned char UCharBinOp_and(unsigned char a, unsigned char b){
  return a&b;
}

unsigned char UCharBinOp_or(unsigned char a, unsigned char b);
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
  for(size_t i=0; i<this->len; ++i){
    this->data[i] = op(this->data[i], that->data[i])
  }
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
