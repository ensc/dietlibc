#ifndef H_DIETLIBC_DIETPAGESIZE_H
#define H_DIETLIBC_DIETPAGESIZE_H

#include <strings.h>
#include "dietfeatures.h"

extern int __libc_getpagesize(void) __attribute__((__const__)) __pure;

#if defined(WANT_STATIC_PAGESIZE)
#  define __DIET_PAGE_SIZE_PREDEF	(WANT_STATIC_PAGESIZE)
#  define __DIET_PAGE_SHIFT_PREDEF	(ffs(__DIET_PAGE_SIZE_PREDEF)-1)
#elif defined(__alpha__) || defined(__sparc__)
#  define __DIET_PAGE_SIZE_PREDEF	(8192UL)
#  define __DIET_PAGE_SHIFT_PREDEF	(13)
#elif defined(__powerpc64__)
#  define __DIET_PAGE_SIZE_PREDEF	(65536UL)
#  define __DIET_PAGE_SHIFT_PREDEF	(16)
#else
#  define __DIET_PAGE_SIZE_PREDEF	(4096UL)
#  define __DIET_PAGE_SHIFT_PREDEF	(12)
#endif

#ifdef WANT_DYN_PAGESIZE
#  define __DIET_PAGE_SIZE	((size_t)__libc_getpagesize())
#  define __DIET_PAGE_SHIFT	(ffs(__DIET_PAGE_SIZE)-1)
#else
#  define __DIET_PAGE_SIZE	__DIET_PAGE_SIZE_PREDEF
#  define __DIET_PAGE_SHIFT	__DIET_PAGE_SHIFT_PREDEF
#endif

#endif	/* H_DIETLIBC_DIETPAGESIZE_H */
