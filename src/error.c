#include "error.h"

void error(char* errorMessage){
  if(errorMessage) fprintf(stderr, "%s\n", errorMessage);
}
