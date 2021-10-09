#ifndef PERROR_EXIT_H
#  define PERROR_EXIT_H

#  ifndef EXIT_FAILURE_H
#    define EXIT_FAILURE_H
#    include "exitFailure.h"
#  endif

/** @brief Print $msg, followed by the last error message encountered during a call to a system
 *  library function.
 *
 *  @param msg The caller-provided message to print, which will be followed by a colon ':', a 
 *  space ' ', and the last error message.
 */

void perrorExit(char* msg);

#endif
