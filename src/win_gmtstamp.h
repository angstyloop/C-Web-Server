#ifndef GMTSTAMP_H
#  define GMTSTAMP_H

#  ifndef WINDOWS_H
#    define WINDOWS_H
#    include <windows.h>
#  endif

#pragma warning(disable:4668) /*if not_defined treated as #if 0*/

#  ifndef SYNCHAPI_H
#    define SYNCHAPI_H
#    include <synchapi.h>
#  endif

#  ifndef TIME_H
#    define TIME_H
#    include <time.h>
#  endif

/** @brief The mutex lock used to prevent simultaneous calls 
 *  to the gmtime function from different threads.
 */
extern HANDLE hIOMutex;

/** @brief Create a timestamp for the current time 
 *  (thread-safe).
 *  
 *  The issue with time.h is that the methods that initialize
 *  the tm structs do work on static-declared objects, which
 *  are stored in the data segment of a compiled C program. 
 *  This means that functions like gmtime are not thread-safe.
 *
 *  A simple workaround is to use a mutex lock to prevent
 *  other threads from calling gmtime at the same time and 
 *  thus accessing the shared memory at the same time.
 *
 *  @return the timestamp, a string formatted after 
 *  RFC 822/1123/2616.
 *
 */
char* win_gmtstamp(void);

#endif