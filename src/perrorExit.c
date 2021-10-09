#include "perrorExit.h"

void perrorExit(char* msg){
  perror(msg);
  exitFailure();
}
