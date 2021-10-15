typedef struct FilePath FilePath;
struct FilePath {
  size_t fullLen;
  size_t parentLen;
  size_t nameLen;
  char* full;
  char* parent;
  char* name;
}

FilePath* FilePath_create(){
  FilePath* this = calloc(1, sizeof(FilePath));
  if(!this){
    perrorExit("(FilePath_create) calloc $this");
  }
  return this;
}

#define FILE_PATH_SEP '\\'

FilePath* FilePath_init(FilePath* this, size_t fullLen, char* full){

  if (!fullLen) return this;

  // Trim leading whitespace (if any), and adjust fullLen  
  //

  // set $p to start of $full
  char* p = full;

  // loop forwards until a non-space is encountered
  for(;;){
    if(!isspace(*p)) break;
    ++p;
  }

  // subtract the number of spaces from $fullLen
  fullLen -= p - full;

  // set $full to the first non-space char
  full = p;


  // Trim trailing whitespace (if any), and adjust fullLen.
  //
  
  // set $p to '\0' pos
  p = full + fullLen;

  // loop backwards until a non-space is encountered
  for(;;){
    if(!isspace(*p)) break;
    --p;
  }

  // subtract the number of spaces from $fullLen
  fullLen -= full + fullLen - p;

  // terminate $full before the spaces
  *(p+1) = 0;
  
  // set $p to pos of last non-null character (just before '\0')
  p = full + fullLen - 1;

  // Remove trailing FILE_PATH_SEP (if any), and adjust fullLen.
  if(*p == FILE_PATH_SEP){
    *p=0; 
    --fullLen;
  }

  // set $p to '\0' pos
  p = full + fullLen;

  // loop backwards until a separator is encountered
  for(;;){
    if(*p == FILE_PATH_SEP){
      sepExists=1;
      break;
    };
    if(p == full) break;
    --p;
  }

  this->fullLen = fullLen;

  // use $p the position of the separator to determine the unknown lengths
  this->parentLen = p - full;
  this->nameLen = fullLen - this->parentLen;

  // use the now known sizes to allocate space
  if(!(this->parent = calloc(parentLen + 1, 1))){
    perrorExit("(FilePath_create) calloc $parent");
  }

  if(!(this->full = calloc(fullLen + 1, 1))){
    perrorExit("(FilePath_create) calloc $full");
  }

  if(!(this->name = calloc(nameLen + 1, 1))){
    perrorExit("(FilePath_create) calloc $name");
  }

  // copy the strings into the allocated space
  strncpy(this->full, full, this->fullLen);
  strncpy(this->parent, parent, this->parentLen);
  strncpy(this->name, name, this->nameLen);
}

FilePath* FilePath_new(FilePath* this){
  
}
