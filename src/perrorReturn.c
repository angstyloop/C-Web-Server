#define perrorReturn(message, returnCode) do{ \
  perror(message); \
  return returnCode; \
}while(0)   
