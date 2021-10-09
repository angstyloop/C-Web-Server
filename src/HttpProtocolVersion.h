/** @file HttpProtocolVersion.h
 *
 *  @brief Each HttpProtocolVersion object holds a unique enum key and a unique version string.
 *
 */

#ifndef HTTP_PROTOCOL_VERSION_H
#define HTTP_PROTOCOL_VERSION_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#define HTTP_PROTOCOL_VERSION_COUNT 4

typedef enum HttpProtocolVersionEnum{
  HTTP_0_9=0,
  HTTP_1_0=1,
  HTTP_1_1=2,
  HTTP_2_0=3
} HttpProtocolVersionEnum;

typedef enum HttpProtocolVersionArray{
  [HTTP_0_9]="HTTP/0.9",
  [HTTP_1_0]="HTTP/1.0",
  [HTTP_1_1]="HTTP/1.1",
  [HTTP_2_0]="HTTP/2.0"
} HttpProtocolVersionArray;

typedef struct HttpProtocolVersion HttpProtocolVersion;
struct HttpProtocolVersion{
  HttpProtocolVersionEnum key;
  const char* value;
};

HttpProtocolVersion* HttpProtocolVersion_create();
HttpProtocolVersion* HttpProtocolVersion_init(HttpProtocolVersion* this, HttpProtocolVersionEnum key);
HttpProtocolVersion* HttpProtocolVersion_new(HttpProtocolVersionKey key);
HttpProtocolVersion* HttpProtocolVersion_free(HttpProtocolVersion* this);
HttpProtocolVersion* HttpProtocolVersion_print(HttpProtocolVersion* this, FILE* out);
HttpProtocolVersion* HttpProtocolVersion_copy(HttpProtocolVersion* this, HttpProtocolVersion* that);
HttpProtocolVersion* HttpProtocolVersion_clone(HttpProtocolVersion* this);

#endif
