typedef struct ByteArray ByteArray;
struct ByteArray{
  size_t len;
  unsigned char* data;
};

ByteArray* ByteArray_create(size_t len);
ByteArray* ByteArray_create(size_t len){
  ByteArray* this = calloc(len, 1);
  this->len = len;
  return this;
}

ByteArray* ByteArray_init(ByteArray* this, unsigned char* data){
  memcpy(this->data, data, this->len);
}

ByteArray* ByteArray_new(size_t len, unsigned char* data);
ByteArray* ByteArray_new(size_t len, unsigned char* data){
  return ByteArray_init(ByteArray_create(len), data);
}

char* ByteArray_printHeader(ByteArray* this, FILE* out);
char* ByteArray_printHeader(ByteArray* this, FILE* out){
  fprintf("%s, %s\n", "len", "data");
  return this;
}

char* ByteArray_printSelf(ByteArray* this, FILE* out);
char* ByteArray_printSelf(ByteArray* this, FILE* out){
  // Print $len and open brace of $data array.
  fprintf(out, "%zu, {", this->len);
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

void ByteArray_lengthCheck(ByteArray* this, ByteArray* that, char* callerName){
  if(this->len != that->len){
    errorExitFailure("(ByteArray%s%s) Lengths don't match.", 
      callerName ? "_" : "", callerName ? callerName : "");
  }
}

ByteArray* ByteArray_applyBinOp(ByteArray* this, ByteArray* that, UCharBinOp op){
  ByteArray_lengthCheck(this, that, "applyBinOp");
  for(size_t i=0; i<this->len; ++i){
    this->data[i] = op(this->data[i], that->data[i])
  }
  return this;
}

ByteArray* ByteArray_binOp(ByteArray* left, ByteArray* right, UCharBinOp op){
  ByteArray_lengthCheck(left, right, "applyBinOp");
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
