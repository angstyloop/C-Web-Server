#include "error.h"

#define errorReturn(message, returnCode) do{ \
  error(message); \
  return returnCode; \
}while(0);
