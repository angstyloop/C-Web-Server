/** @file HttpStatus.h
 *  @brief The HttpStatus object interface. 
 *
 *  The HttpStatus object contains the $code and $text of an HTTP status.
 *
 *  @author Sean Allen (sean.christian.allen.ii@gmail.com)
 *  @date 2021-10-03
 */

#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#include "returnSuccess.h"

/** #def HTTP_STATUS_COUNT Number of HTTP possible statuses as of today.
 */
#define HTTP_STATUS_COUNT 63

/** @enum HttpStatusEnum
 *  @brief An enum for the HTTP statuses.
 */
typedef enum HttpStatusEnum{
  CONTINUE_100=0,
  SWITCHING_PROTOCOLS_101=1,
  PROCESSING_102=2,
  OK_200=3,
  CREATED_201=4,
  ACCEPTED_202=5,
  NON_AUTHORITATIVE_INFORMATION_203=6,
  NO_CONTENT_204=7,
  RESET_CONTENT_205=8,
  PARTIAL_CONTENT_206=9,
  MULTI_STATUS_207=10,
  ALREADY_REPORTED_208=11,
  IM_USED_226=12,
  MULTIPLE_CHOICES_300=13,
  MOVED_PERMANENTLY_301=14,
  FOUND_302=15,
  SEE_OTHER_303=16,
  NOT_MODIFIED_304=17,
  USE_PROXY_305=18,
  TEMPORARY_REDIRECT_307=19,
  PERMANENT_REDIRECT_308=20,
  BAD_REQUEST_400=21,
  UNAUTHORIZED_401=22,
  PAYMENT_REQUIRED_402=23,
  FORBIDDEN_403=24,
  NOT_FOUND_404=25,
  METHOD_NOT_ALLOWED_405=26,
  NOT_ACCEPTABLE_406=27,
  PROXY_AUTHENTICATION_REQUIRED_407=28,
  REQUEST_TIMEOUT_408=29,
  CONFLICT_409=30,
  GONE_410=31,
  LENGTH_REQUIRED_411=32,
  PRECONDITION_FAILED_412=33,
  PAYLOAD_TOO_LARGE_413=34,
  REQUEST_URI_TOO_LONG_414=35,
  UNSUPPORTED_MEDIA_TYPE_415=36,
  REQUESTED_RANGE_NOT_SATISFIABLE_416=37,
  EXPECTATION_FAILED_417=38,
  IM_A_TEAPOT_418=39,
  MISDIRECTED_REQUEST_421=40,
  UNPROCESSABLE_ENTITY_422=41,
  LOCKED_423=42,
  FAILED_DEPENDENCY_424=43,
  UPGRADE_REQUIRED_426=44,
  PRECONDITION_REQUIRED_428=45,
  TOO_MANY_REQUESTS_429=46,
  REQUEST_HEADER_FIELDS_TOO_LARGE_431=47,
  CONNECTION_CLOSED_WITHOUT_RESPONSE_444=48,
  UNAVAILABLE_FOR_LEGAL_REASONS_451=49,
  CLIENT_CLOSED_REQUEST_499=50,
  INTERNAL_SERVER_ERROR_500=51,
  NOT_IMPLEMENTED_501=52,
  BAD_GATEWAY_502=53,
  SERVICE_UNAVAILABLE_503=54,
  GATEWAY_TIMEOUT_504=55,
  HTTP_VERSION_NOT_SUPPORTED_505=56,
  VARIANT_ALSO_NEGOTIATES_506=57,
  INSUFFICIENT_STORAGE_507=58,
  LOOP_DETECTED_508=59,
  NOT_EXTENDED_510=60,
  NETWORK_AUTHENTICATION_REQUIRED_511=61,
  NETWORK_CONNECT_TIMEOUT_ERROR_599=62
} HttpStatusEnum;

#define HTTP_STATUS_TEXT_MAX_SIZE 64

/** @struct HttpStatus 
 *  @property HttpStatus::code The unique 3-digit HTTP status code.
 *  @property HttpStatus::text (immutable) The unique text corresponding to the HTTP status $code.  
 */
typedef struct HttpStatus HttpStatus;
struct HttpStatus{
  int code;
  char text[64];
};

/** @brief HttpStatusArray maps the enums to HttpStatus objects.
 * 
 *  In case you forgot:
 *
 *  1xx Informational
 *  2xx Success
 *  3xx Redirection
 *  4xx Client Error
 *  5xx Server Error
 *
 */
#define HttpStatusArray /**/ \
(HttpStatus[HTTP_STATUS_COUNT]){ \
  [CONTINUE_100]={100, "Continue"}, \
  [SWITCHING_PROTOCOLS_101]={101, "Switching Protocols"}, \
  [PROCESSING_102]={102, "Processing"}, \
  [OK_200]={200, "OK"}, \
  [CREATED_201]={201, "Created"}, \
  [ACCEPTED_202]={202, "Accepted"}, \
  [NON_AUTHORITATIVE_INFORMATION_203]={203, "Non_authoritative Information"}, \
  [NO_CONTENT_204]={204, "No Content"}, \
  [RESET_CONTENT_205]={205, "Reset Content"}, \
  [PARTIAL_CONTENT_206]={206, "Partial Content"}, \
  [MULTI_STATUS_207]={207, "Multi_Status"}, \
  [ALREADY_REPORTED_208]={208, "Already Reported"}, \
  [IM_USED_226]={226, "I'm Used"}, \
  [MULTIPLE_CHOICES_300]={300, "Multiple Choices"}, \
  [MOVED_PERMANENTLY_301]={301, "Moved Permanently"}, \
  [FOUND_302]={302, "Found"}, \
  [SEE_OTHER_303]={303, "See Other"}, \
  [NOT_MODIFIED_304]={304, "Not Modified"}, \
  [USE_PROXY_305]={305, "Use Proxy"}, \
  [TEMPORARY_REDIRECT_307]={307, "Temporary Redirect"}, \
  [PERMANENT_REDIRECT_308]={308, "Permanent Redirect"}, \
  [BAD_REQUEST_400]={400, "Bad Request"}, \
  [UNAUTHORIZED_401]={401, "Unauthorized"}, \
  [PAYMENT_REQUIRED_402]={402, "Payment Required"}, \
  [FORBIDDEN_403]={403, "Forbidden"}, \
  [NOT_FOUND_404]={404, "Not Found"}, \
  [METHOD_NOT_ALLOWED_405]={405, "Method Not Allowed"}, \
  [NOT_ACCEPTABLE_406]={406, "Not Acceptable"}, \
  [PROXY_AUTHENTICATION_REQUIRED_407]={407, "Proxy Authentication Required"}, \
  [REQUEST_TIMEOUT_408]={408, "Request Timeout"}, \
  [CONFLICT_409]={409, "Conflict"}, \
  [GONE_410]={410, "Gone"}, \
  [LENGTH_REQUIRED_411]={411, "Length Required"}, \
  [PRECONDITION_FAILED_412]={412, "Precondition Failed"}, \
  [PAYLOAD_TOO_LARGE_413]={413, "Payload Too Large"}, \
  [REQUEST_URI_TOO_LONG_414]={414, "Request URI Too Long"}, \
  [UNSUPPORTED_MEDIA_TYPE_415]={415, "Unsupported Media Type"}, \
  [REQUESTED_RANGE_NOT_SATISFIABLE_416]={416, "Requested Range Not Satisfiable"}, \
  [EXPECTATION_FAILED_417]={417, "Expectation Failed"}, \
  [IM_A_TEAPOT_418]={418, "I'm a teapot"}, \
  [MISDIRECTED_REQUEST_421]={421, "Misdirected Request"}, \
  [UNPROCESSABLE_ENTITY_422]={422, "Unprocessable Entity"}, \
  [LOCKED_423]={423, "Locked"}, \
  [FAILED_DEPENDENCY_424]={424, "Failed Dependency"}, \
  [UPGRADE_REQUIRED_426]={426, "Upgrade Required"}, \
  [PRECONDITION_REQUIRED_428]={428, "Precondition Required"}, \
  [TOO_MANY_REQUESTS_429]={429, "Too Many Requests"}, \
  [REQUEST_HEADER_FIELDS_TOO_LARGE_431]={431, "Request Header Fields Too Large"}, \
  [CONNECTION_CLOSED_WITHOUT_RESPONSE_444]={444, "Connection Closed Without Response"}, \
  [UNAVAILABLE_FOR_LEGAL_REASONS_451]={451, "Unavailable For Legal Reasons"}, \
  [CLIENT_CLOSED_REQUEST_499]={499, "Client Closed Request"}, \
  [INTERNAL_SERVER_ERROR_500]={500, "Internal Server Error"}, \
  [NOT_IMPLEMENTED_501]={501, "Not Implemented"}, \
  [BAD_GATEWAY_502]={502, "Bad Gateway"}, \
  [SERVICE_UNAVAILABLE_503]={503, "Service Unavailable"}, \
  [GATEWAY_TIMEOUT_504]={504, "Gateway Timeout"}, \
  [HTTP_VERSION_NOT_SUPPORTED_505]={505, "HTTP Version Not Supported"}, \
  [VARIANT_ALSO_NEGOTIATES_506]={506, "Variant Also Negotiates"}, \
  [INSUFFICIENT_STORAGE_507]={507, "Insufficient Storage"}, \
  [LOOP_DETECTED_508]={508, "Loop Detected"}, \
  [NOT_EXTENDED_510]={510, "Not Extended"}, \
  [NETWORK_AUTHENTICATION_REQUIRED_511]={511, "Network Authentication Required"}, \
  [NETWORK_CONNECT_TIMEOUT_ERROR_599]={599, "Network Connect Timeout Error"} \
}

/** @brief Create a new empty HttpStatus.
 */
HttpStatus* HttpStatus_create(void);

/** @brief Initialize this HttpStatus from an HttpStatusEnum.
 */
HttpStatus* HttpStatus_init(HttpStatus*, HttpStatusEnum);

/** @brief Create a new initialized HttpStatus from an HttpStatusEnum.
 */
HttpStatus* HttpStatus_new(HttpStatusEnum);

/** @brief Free an HttpStatus.
 */
HttpStatus* HttpStatus_free(HttpStatus*);

/** @brief Copy a source HttpStatus into this HttpStatus. This is a deep copy.
 */
HttpStatus* HttpStatus_copy(HttpStatus*, HttpStatus*);

/** @brief Clone a this HttpStatus. This is a deep copy.
 */
HttpStatus* HttpStatus_clone(HttpStatus*);

/** @brief Initialize this HttpStatus from a 3-digit decimal integer HTTP status code.
 */
HttpStatus* HttpStatus_initByCode(HttpStatus* this, int code);

/** @brief Create a new initialized HttpStatus from a 3-digit decimal integer HTTP status code. Returns null if none found.
 */
HttpStatus* HttpStatus_newByCode(int code);

/** @brief Write the member values of $this HttpStatus object to an output stream $out.
 */
HttpStatus* HttpStatus_writeObject(HttpStatus* this, FILE* out);

/** @brief Create a new string representation of the HttpStatus, suitable for use in the first line of an HttpResponse. Must be freed.
 */
char* HttpStatus_getStatusString(HttpStatus* this);

/** @brief Write the string representation of $this HttpStatus to an output stream $out.
 */
HttpStatus* HttpStatus_writeStatusString(HttpStatus* this, FILE* out);

/** @brief Write the member values of each object in an array $objects to an output stream $out.
 */
HttpStatus** HttpStatus_writeObjects(HttpStatus** objects, FILE* out);

/** @brief Free each HttpStatus in an array.
 */
HttpStatus** HttpStatus_freeAll(HttpStatus**);

#endif
