#include "errorExit.h"

void errorExit(char* errorMessage, int exitCode){
  error(errorMessage);
  exit(exitCode);
}
