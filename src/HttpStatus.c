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

void HttpStatus_printHeader(FILE* out){
  fprintf(out, "type, code, text\n");
}

HttpStatus* HttpStatus_print(HttpStatus* this, FILE* out){
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

HttpStatus** HttpStatus_printAll(HttpStatus** all, FILE* out){
  while(*all) HttpStatus_print(*all++, out);
  return all;
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
  HttpStatus_printHeader(stdout);
  puts("You should be seeing two of everything.");
  for(int e=0; e<HTTP_STATUS_COUNT; ++e){
    status[0] = HttpStatus_new(e);
    assert(HttpStatus_initByCode(status[1] = HttpStatus_create(), status[0]->code));
    assert(status[0]->code == status[1]->code);
    assert(!strcmp(status[0]->text, status[1]->text));
    assert(!HttpStatus_cmp(status[0], status[1]));
    HttpStatus_printAll(status, stdout);
    HttpStatus_freeAll(status);
  }
  puts("You should be seeing two of everything.");
  puts("OK");
  returnSuccess();
}
#endif

/*
cl /Wall /Zi /D _NO_CRT_STDIO_INLINE /D TEST_HTTP_STATUS /Fe:test-http-status HttpStatus.c
*/
