/** @file HttpStatus.h
 */

#include "HttpStatus.h"

HttpStatus* HttpStatus_create(void){
  HttpStatus* this = calloc(1, sizeof(HttpStatus));
  return this;
}

HttpStatus* HttpStatus_init(HttpStatus* this, HttpStatusEnum e){
  *this = HttpStatusArray[e];
  return this;
}

HttpStatus* HttpStatus_new(HttpStatusEnum e){
  return HttpStatus_init(HttpStatus_create(), e);
}

HttpStatus* HttpStatus_free(HttpStatus* this){
  free(this);
  return this;
}

HttpStatus* HttpStatus_copy(HttpStatus* this, HttpStatus* that){
  *this = *that;
  return this;
}

HttpStatus* HttpStatus_clone(HttpStatus* this){
  return HttpStatus_copy(HttpStatus_create(), this);
}

HttpStatus* HttpStatus_initByCode(HttpStatus* this, int code){
  HttpStatus temp={0};
  for(int i=0; i<HTTP_STATUS_COUNT; ++i){
    if((temp = HttpStatusArray[i]).code == code){
      *this = temp;
      return this;
    }
  }
  return 0;
}

HttpStatus* HttpStatus_newByCode(int code){
  return HttpStatus_initByCode(HttpStatus_create(), code);
}

void HttpStatus_writeObjectHeader(FILE* out){
  fprintf(out, "type, code, text\n");
}

HttpStatus* HttpStatus_writeObject(HttpStatus* this, FILE* out){
  fprintf(out, "HttpStatus, %d, %s\n", this->code, this->text);
  return this;
}

int HttpStatus_cmp(HttpStatus* this, HttpStatus* that){
  if(this == that) return 0;
  // compare by code
  else if(this->code < that->code) return -1;
  else if(that->code < this->code) return 1;
  // then by string
  else if(0>strncmp(this->text, that->text, HTTP_STATUS_TEXT_MAX_SIZE)) return -1;
  else if(0<strncmp(this->text, that->text, HTTP_STATUS_TEXT_MAX_SIZE)) return 1;
  else return 0;
}

char* HttpStatus_getStatusString(HttpStatus* this){
  // Doesn't use snprintf because it may not be safely implemented (e.g. an alias to sprintf) 
  // or may not even be available (VisualStudio).

  size_t textLength = strlen(this->text);
  size_t statusStringSize = 3/*code*/ + 1/*space*/ + textLength/*text*/ + 1/*nullbyte*/;
  char* statusString = calloc(statusStringSize, 1), * p = statusString;

  char code[3]={0};
  _itoa(this->code, code, 10);
  memcpy(p, this->code, 3);
  p+=3;
  *p++ = ' ';
  memcpy(p, this->text, textLength);
  p+=textLength;
  *p=0;
  return statusString;
}

HttpStatus* HttpStatus_writeStatusString(HttpStatus* this, FILE* out){
  fprintf(out, "%d %s", this->code, this->text);
  return this;
}

HttpStatus** HttpStatus_writeObjects(HttpStatus** objects, FILE* out){
  while(*objects) HttpStatus_writeObject(*objects++, out);
  return objects;
}

HttpStatus** HttpStatus_freeAll(HttpStatus** all){
  while(*all) HttpStatus_free(*all++);
  return all;
}

/** Test
 */

#ifdef TEST_HTTP_STATUS
#ifndef ASSERT_H
#define ASSERT_H
#include <assert.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

int main(void){
  HttpStatus* status[3]={0};
  HttpStatus_writeObjectHeader(stdout);
  puts("TEST_HTTP_STATUS");
  for(int e=0; e<HTTP_STATUS_COUNT; ++e){
    status[0] = HttpStatus_new(e);
    puts("_initByCode");
    status[1] = HttpStatus_newByCode(status[0]->code);
    assert(status[1]);
    assert(status[0]->code == status[1]->code);
    assert(!strcmp(status[0]->text, status[1]->text));
    puts("_cmp");
    assert(!HttpStatus_cmp(status[0], status[1]));
    puts("_free");
    HttpStatus_free(status[1]);
    puts("_clone");
    status[1] = HttpStatus_clone(status[0]);
    assert(!HttpStatus_cmp(status[0], status[1]));
    puts("_writeObjects");
    HttpStatus_writeObjects(status, stdout);
    puts("_getStatusString");
    char* str = HttpStatus_getStatusString(status[0]);
    printf("%s\n", str);
    free(str);
    HttpStatus_writeStatusString(status[0], stdout);
    puts("_writeObject");
    HttpStatus_writeObject(status[0], stdout);
    puts("_freeAll");
    HttpStatus_freeAll(status);
  }
  puts("OK");
  returnSuccess;
}
#endif

/*
cl /Wall /Zi /D _NO_CRT_STDIO_INLINE /D TEST_HTTP_STATUS /Fe:test-http-status HttpStatus.c
*/
