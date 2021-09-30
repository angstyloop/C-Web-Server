#ifndef WINDOWS_MUTEX_EXAMPLE_H
#  define WINDOWS_MUTEX_EXAMPLE_H

#  ifndef WINDOWS_H
#    define WINDOWS_H
#    include <windows.h>
#  endif

#  ifndef STDIO_H
#    define STDIO_H
#    include <stdio.h>
#  endif

#  ifndef SYNCHAPI_H
#    define SYNCHAPI_H
#    include <synchapi.h>
#  endif

extern HANDLE hIOMutex;
extern unsigned poop;

void windows_mutex_example();

#endif
