#ifndef MESSAGE_RETURN_SUCCESS
#define MESSAGE_RETURN_SUCCESS

#include "messageReturn.h"

#define messageReturnSuccess(message) do{ \
  messageReturn(message, EXIT_SUCCESS); \
}while(0)

#endif
