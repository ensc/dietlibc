#ifndef _STDINT_H
#define _STDINT_H

#include <inttypes.h>

__BEGIN_DECLS

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;

#ifndef __STRICT_ANSI__
typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;
#endif

typedef signed char int_fast8_t;
typedef signed long int int_fast16_t;
typedef signed long int int_fast32_t;
typedef signed long long int int_fast64_t;

typedef unsigned char uint_fast8_t;
typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long long int uint_fast64_t;

#include <endian.h>

__END_DECLS

#endif
