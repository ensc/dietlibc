#ifndef _ASM_TYPES_H
#define _ASM_TYPES_H

#include <sys/types.h>

#ifdef __alpha__
typedef unsigned int umode_t;
#else
typedef unsigned short umode_t;
#endif

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
#ifndef __STRICT_ANSI__
typedef uint64_t __u64;
#endif

typedef int8_t __s8;
typedef int16_t __s16;
typedef int32_t __s32;
#ifndef __STRICT_ANSI__
typedef int64_t __s64;
#endif

#if defined(__alpha__)
typedef unsigned long __kernel_size_t;
#else
typedef unsigned int __kernel_size_t;
#endif

#endif
