#ifndef RETURN_SUCCESS_H
#define RETURN_SUCCESS_H

#include "__stdlib.h"

/** @macro returnSuccess
 *  @brief A macro that simply returns EXIT_SUCCESS.
 */
#define returnSuccess do{return EXIT_SUCCESS;}while(0)

#endif
