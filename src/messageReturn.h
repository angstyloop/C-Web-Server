#ifndef MESSAGE_RETURN_H
#define MESSAGE_RETURN_H

#define messageReturn(message, returnCode) do{ \
  fprintf(stdout, "%s\n", message); \
  return returnCode; \
}while(0)

#endif
