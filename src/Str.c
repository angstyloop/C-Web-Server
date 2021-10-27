#include  "Str.h"

Str* Str_create(size_t size){
  Str* this = calloc(1, sizeof(Str));
  if(!this)
    perrorExit("(Str_create) calloc $this");

  if(!(this->data = calloc(size + 1, 1))){
    free(this);
    perrorExit("(Str_create) calloc $this->size");
  }

  this->size = size;

  return this;
}

Str* Str_free(Str* this){
  secure_memzero(this->data, this->size);
  free(this->data);
  secure_memzero(this);
  free(this);
  return this;
}

Str* Str_fillRange(Str* this, size_t start, size_t end, char byte){
  const size_t end_ = end > this->len ? end : this->len;
  for(size_t start=0; i<end; ++i)
    this->data[i] = byte;
  return this;
}

Str* Str_resize(Str* this, size_t size){
  if(this->size == size) return this;
  if(!size) return Str_free(this);
  char* temp = realloc(this->data, size);
  if(!temp){
    Str_free(this);
    perrorExit("realloc");
  }
  this->data = temp;
  this->size = size;

  if(size < this->len){
    /* If truncating, set the length to one less than the new size, and write a 
     * terminating null byte to the new end position.
     */
    this->len = size-1;
    this->data[size-1]=0;
  }
}

Str* Str_growUntil(Str* this, size_t len){}

Str* Str_init(Str* this, size_t len, char* data){
  if(!this || !this->size){
    errorExit("(Str_init) Must create $this before init.");
  }
}

Str* Str_new(){}

Str* Str_print(){}

Str* Str_concat(){}

Str* Str_append(){}

Str* Str_interp(){}

Str* Str_copy(){}

Str* Str_move(){}

Str* Str_growByFactor(){}

Str* Str_shrinkByFactor(){}

Str* Str_growToExceed(){}

Str* Str_shrink(){}

Str* Str_trim(){}

Str* Str_getDelim(){}

Str* Str_read(){}

Str* Str_write(){}
