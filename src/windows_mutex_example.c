#include "windows_mutex_example.h"

HANDLE hIOMutex={0};
unsigned poop=0;

void windows_mutex_example(){
  WaitForSingleObject(hIOMutex, INFINITE);
  poop++;
  ReleaseMutex(hIOMutex);
}

#ifdef TEST_WINDOWS_MUTEX_EXAMPLE
int main(void){
  hIOMutex = CreateMutex(0, 0, 0);
  windows_mutex_example();
  return EXIT_SUCCESS;
}
#endif
/*
cl /Zi /D TEST_WINDOWS_MUTEX_EXAMPLE /Fe:test-windows-mutex-example windows_mutex_example.c
*/
