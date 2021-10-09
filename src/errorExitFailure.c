#include "errorExitFailure.h"

void errorExitFailure(char* errorMessage){
  errorExit(errorMessage, EXIT_FAILURE);
}
