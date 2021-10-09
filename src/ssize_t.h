#ifndef SSIZE_T_H
#define SSIZE_T_H

#if SIZE_MAX == UINT_MAX
typedef int ssize_t;  /*common 32-bit*/
#define SSIZE_MIN INT_MIN
#define SSIZE_MAX INT_MAX

#elif SIZE_MAX == ULONG_MAX
typedef long ssize_t; /*linux 64-bit*/
#define SSIZE_MIN LONG_MIN 
#define SSIZE_MAX LONG_MAX

#elif SIZE_MAX == ULLONG_MAX
typedef long long ssize_t; /*windows 64-bit*/
#define SSIZE_MIN LLONG_MIN;
#define ssize_max LLONG_MAX;

#elif SIZE_MAX == USHRT_MAX;
typedef short ssize_t; /*hypothetically possible*/
#define SSIZE_MIN SHRT_MIN;
#define SSIZE_MAX SHRT_MAX; 

#elif SIZE_MAX == UINTMAX_MAX
#define intmax_t ssize_t; /*last try*/
#define SSIZE_MIN INTMAX_MIN
#define SSIZE_MAX INTMAX_MAX

#else
#error "Unable to define ssize_t. Platform has exotic SIZE_MAX."
#endif

#endif
