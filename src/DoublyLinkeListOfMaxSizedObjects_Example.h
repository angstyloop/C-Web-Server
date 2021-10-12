#ifndef MY__STRING_H
#define MY__STRING_H

// Are your string objects limited to a size smaller than the memory available to your process, due to a specification like WASM? This may be for you.

void exit_failure(){
  exit(EXIT_FAILURE);
}

void perror_exit(char* message){
  perror(message);
  exit_failure();
}

typedef struct CharArray CharArray;
struct CharArray {
  size_t size;
  size_t len;
  char* data;
};

CharArray* CharArray_create(size_t size){
  CharArray* this = calloc(1, sizeof(CharArray));
  this->size =  size;
  this->len = size-1;
  this->data = calloc(size, 1);
  return this;
}

CharArray* CharArray_init(CharArray* this, char* data){
  memcpy(this->data, data, this->len);
  return this;
}

CharArray* CharArray_new(size_t size, char* data){
  return CharArray_init(CharArray_create(size), data);
}

CharArray* CharArray_free(CharArray* this){
  free(this->data); 
  this->size=0; 
  this->len=0;
  this->data=0;
  free(this);
  return this;
}

CharArray* CharArray_size(CharArray* this){
  return 2*sizeof(size_t) + size + 2*sizeof(CharArray*);
}

CharArray* CharArray_copy(CharArray* this, CharArray* source){
  if(this != source){
    this->len = source->len;
    memcpy(this->data, source->data, source->len);
  }
  return this;
}

CharArray* CharArray_clone(CharArray* this){
  CharArray* clone = CharArray_create(this->size);
  return CharArray_copy(clone, this);
}

CharArray* CharArray_print(this, FILE* out){
  fprintf(out, "size,len,data");
  fprintf(out, "%zu,%zu,%s", this->size, this->len, this->data);
  return this;
}

int CharArray_read(CharArray* this, FILE* in){
  size_t z=0;
  z = fread(this->data, 1, SIZE_MAX-1, in);
  this->data[z]=0;
  return SIZE_MAX == z;
}

typedef struct DLLNode DLLNode;
struct DLLNode{
  void* data;
  DLLNode* next;
  DLLNode* prev;
};

DLLNode* DLLNode_create(){
  DLLNode* this = calloc(1, sizeof(DLLNode));
  this->next = this;
  this->prev = this;
  return this;
}

DLLNode* DLLNode_init(DLLNode* this, void* data){
  if(data) this->data = data;
  return this;
}

DLLNode* DLLNode_new(void* data){
  return DLLNode_init(DLLNode_create(), data);
}

DLLNode* DLLNode_free(DLLNode* this){
  p->data=0;
  p->next=0;
  p->prev=0;
  free(this);
  return this;
}

DLLNode* DLLNode_freeAll(DLLNode* this){
  DLLNode* p=this, * q=this->next;
  do{
    DLLNode_free(p);
    p=q;
  }while(q=this->next);
  return this;
}

DLLNode* DLLNode_insertNext(DLLNode* this, DLLNode* next){
  this->next->prev = next;
  this->next = next;
  return next;
}

DLLNode* DLLNode_append

DLLNode* DLLNode_insertPrevious(DLLNode* this, DLLNode* previous){
  this->previous->next = previous;
  this->previous = previous;
  return previous;
}

DLLNode* DLLNode_remove(DLLNode* this){
  this->next->previous = this->previous;
  this->previous->next = this->next;
  return this;
}

DLLNode* DLLNode_delete(DLLNode* this){
  DLLNode_remove(this);
  return DLLNode_free(this);
}

DLLNode* DLLNode_copy(DLLNode* this, DLLNode* source){
  this->data = source->data;
  this->previous = source->previous;
  this->next = source->next;
  return this;
}

DLLNode* DLLNode_clone(DLLNode* this){
  return DLLNode_copy(DLLNode_create(), this);
}

DLLNode* DLLNode_cloneAll(DLLNode* this){
  DLLNode* clone = DLLNode_clone(this), * start = this, * p=this;
  while((p = p->next) != start) clone = DLLNode_insertNext(clone, DLLNode_clone(p));
  return clone;
}

DLLNode* DLLNode_print(DLLNode* this, char* (*toString)(void*),  FILE* out){
  fprintf(out, "data: %s", toString(this->data));
  return this;
}

DLLNode* DLLNode_add(DLLNode* this, DLLNode* newNode){
  return DLLNode_insertNext(this->previous, newNode);
  return this;
}

typedef struct String String;
struct String{
  unsigned size;
  unsigned len;
  DLLNode* node;
};

String* String_create(){
  String* this = calloc(1, sizeof(String));
  this->node = DLLNode_create();
  return this;
}

String* String_free(String* this){
  freeAll(this->node);
  this->size=0;
  this->len=0;
  this->node=0;
  free(this);
  return this;
}

String* String_init(String* this, size_t size, char* source){
  this->size = size;
  this->len = size ? size-1 : 0;
  DLLNode_init(this->node, (void*)CharArray_new(size, source));
  return this;
}

String_new(size_t size, char* source){
  return String_init(String_create(), size, source);
}

String* String_read(String* this, FILE* in){
  DLLNode* p = this->node;
  while(CharArray_read((CharArray*)p->data, in)) 
    p = DLLNode_insertNext(p, DLLNode_create());
  return this;
}

/** @brief Append $size-1 bytes from byte array $source to a String, followed by a terminating 
 *         null byte. $source must be at least $sourceSize bytes in length. 
 *
 *  @param this A pointer to the String object being modified.
 *
 *  @param size The size of a buffer just large enough to hold the bytes written and a terminating
 *         null byte.
 *
 *  @param source The source byte array.
 *
 *  @return A pointer to the String object being modified. 
 */
String* String_add(String* this, size_t size, char* source){

  // If no bytes will be written, we're done.
  if(!size) return this;
  
  // If source size is less than or equal to the remaining space in the last 
  // node, re-allocate enough space in the last node's character array for source, 
  // and write the entire source, including the terminating null byte.

  CharArray* charArray = (CharArray*)this->node->previous->data;
  char* temp=0;

  // $source and the pre-existing CharArray::$data both have terminating null bytes - don't count 
  // them both, since the pre-existing null byte will be overwritten.
  size_t remaining = SIZE_MAX - (charArray->size - 1);

  if(size <= remaining){
    if(!(temp = realloc(charArray->data, charArray->size - 1 + size))){
      String_free(this);
      perror_exit("realloc");
    }
    charArray->data = temp;
    memcpy(charArray->data + charArray->size - 1, source, size);
    return this;
  }

  // Otherwise, allocate the rest of the last node (the character array can only be
  // SIZE_MAX), and write that many bytes from source, decrement source size, and
  // increment the source pointer.
 
  if(!(temp = realloc(charArray->data, SIZE_MAX))){
    String_free(this);
    perror_exit("realloc");
  }
  charArray->data = temp;
  memcpy(charArray->data + charArray->size-1, source, remaining-1);
  size -= remaining-1;
  source += remaining-1;
  charArray->data[SIZE_MAX-1]=0;

  // Then, while source size is greater than SIZE_MAX, write SIZE_MAX-1 bytes
  // from source to a new node's full SIZE_MAX char array (the last byte is the null byte), 
  // and decrement source size by SIZE_MAX-1. 

  DLLNode* endNode = this->previous;
  while(SIZE_MAX < size){
    endNode = DLLNode_insertNext(endNode, DLLNode_new((void*)CharArray_new(SIZE_MAX, source)));
    size -= SIZE_MAX-1;
    source += SIZE_MAX-1;
  }

  // Then write the remaining bytes (which are fewer than SIZE_MAX now) to a final new node.

  DLLNode_insertNext(endNode, DLLNode_new(void*)CharArray_new(size, source));

  // All done!

  return this;
}

char String String_char(String* this, unsigned index){
  DLLNode* node = this->node;
  while(SIZE_MAX<=index){
    node = node->next;
    if(node == this->node){
      fprintf(stderr, "Index out of bounds.\r\nIndex: %zu\r\nString:\r\n", SIZE_MAX);
      String_print(this);
      exit_failure();
    }
    index -= SIZE_MAX;
  }
}

String* String_copy(String* this, String* that){}

String* String_clone(String* this){}

String* String_print(String* this){}

String* String_size(String* this){}

unsigned int long String_size(String* this){
  unsigned long count=0;
  DLLNode* p = this->node;
  do{++count;}
  while((p = p->next) != this->node);  
  return (count-1)*(unsigned long)SIZE_MAX + ((String*)(p->previous->data))->size;
}

typedef struct Predicate Predicate;
struct Predicate{
  int (*function)(void*);
};

Predicate* Predicate_create(){}
Predicate* Predicate_init(Predicate* this){}
Predicate* Predicate_new(Predicate* this){}
Predicate* Predicate_free(Predicate* this){}
Predicate* Predicate_copy(Predicate* this){}
Predicate* Predicate_clone(Predicate* this){}
Predicate* Predicate_print(Predicate* this){}
Predicate* Predicate_size(Predicate* this){}

typedef struct Invariant Invariant;
struct Invariant{
  CharArray clazz;
  CharArray description;
  int predicate(void*);
};

Invariant* Invariant_create(){}
Invariant* Invariant_init(Invariant* this){}
Invariant* Invariant_new(Invariant* this){}
Invariant* Invariant_free(Invariant* this){}
Invariant* Invariant_copy(Invariant* this){}
Invariant* Invariant_clone(Invariant* this){}
Invariant* Invariant_print(Invariant* this){}

size_t Invariant_size(Invariant* this){
  return CharArray_size(this->description);
  return sizeof(int (*_)(void*));
}

Invariant* Invariant_create(size_t szDescription){
  Invariant* this = calloc(1, sizeof(invariant));
  this->description = calloc(szDescription, 1);
  return this;
}

Invariant* Invariant_init(Invariant* this, CharArray* description, int (*predicate)(void*)){
  CharArray_copy(this->description, description);
}

Invariant* Invariant_new(Invariant* this, CharArray description, int (*predicate)(void*)){}

Invariant* Invariant_free(Invariant* this, CharArray){}

Invariant* Invariant_print(Invariant* this, FILE* out){
  fprintf("description: %s", this->description->value, );
  return this;
}

#endif
