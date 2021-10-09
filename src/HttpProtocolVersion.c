/** @file HttpProtocolVersion.c
 */

#include "HttpProtocolVersion.h"

HttpProtocolVersion* HttpProtocolVersion_create(){
  HttpProtocolVersion* this = calloc(1, sizeof(HttpProtocolVersion*));
  return this;
}

HttpProtocolVersion* HttpProtocolVersion_init(HttpProtocolVersion* this, HttpProtocolVersionEnum key){
  this->key = key;
  this->value = HttpProtocolVersionArray[key];
  return this
}

HttpProtocolVersion* HttpProtocolVersion_new(HttpProtocolVersionKey key){
  return HttpProtocolVersion_init(HttpProtocolVersion_create(), key);
}

HttpProtocolVersion* HttpProtocolVersion_free(HttpProtocolVersion* this){
  free(this);
  return this;
}

HttpProtocolVersion* HttpProtocolVersion_print(HttpProtocolVersion* this, FILE* out){
  fprintf(out, "HttpProtocolVersion: { key: %zu, value: %s }\n", this->key, this->value);
  return this;
}

HttpProtocolVersion* HttpProtocolVersion_copy(HttpProtocolVersion* this, HttpProtocolVersion* that){
  this->key = that->key;
  this->value = that->value;
  return this;
}

HttpProtocolVersion* HttpProtocolVersion_clone(HttpProtocolVersion* this){
  return HttpProtocolVersion_copy(HttpProtocolVersion_create(), this);
}

#ifndef TEST_HTTP_PROTOCOL_VERSION
int main(){
  HttpProtocolVersion_free(HttpProtocolVersion_print(HttpProtocolVersion_clone(HttpProtocolVersion_new(HTTP_0_9))));
  HttpProtocolVersion_free(HttpProtocolVersion_print(HttpProtocolVersion_clone(HttpProtocolVersion_new(HTTP_1_0))));
  HttpProtocolVersion_free(HttpProtocolVersion_print(HttpProtocolVersion_clone(HttpProtocolVersion_new(HTTP_1_1))));
  HttpProtocolVersion_free(HttpProtocolVersion_print(HttpProtocolVersion_clone(HttpProtocolVersion_new(HTTP_2_0))));
  return EXIT_SUCCESS;
}
#endif

/*
WIN
cl /Zi /Wall /Fe:test-http-protocol-version /D:TEST_HTTP_PROTOCOL_VERSION HttpProtocolVersion.c
*/
