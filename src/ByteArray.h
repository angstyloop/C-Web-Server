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

ByteArray* ByteArray_newByCopy(size_t len, unsigned char* data);
ByteArray* ByteArray_newByCopy(size_t len, unsigned char* data){
  return ByteArray_init(ByteArray_create(len), data);
}

ByteArray* ByteArray_newByMove(size_t len, unsigned char* data);
ByteArray* ByteArray_newByMove(size_t len, unsigned char* data){
  ByteArray* this = ByteArray_create(len);
  this->data = data;
  return this;
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
  return countOneBits(this->data, size_t len);
}
