#ifndef _SYS_ATOMIC_H
#define _SYS_ATOMIC_H

#include <stddef.h>
#include <sys/cdefs.h>

/* this file defines __CAS (compare and swap) and __atomic_add */

#if defined(__INTEL_COMPILER) || (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1))

/* recent gcc versions and the intel compiler have built-ins for this */
#define __CAS(ptr,oldval,newval) __sync_val_compare_and_swap(ptr,oldval,newval)

#elif defined(__arm__)

/* The situation with atomic instructions on ARM is horrible.
 * So much so that the Linux kernel is offering an undocumented
 * interface for a cmpxchg emulation that works on the current kernel.
 * You reach it by jumping to the address 0xffff0fc0 */
typedef int (__kernel_cmpxchg_t)(int oldval, int newval, int *ptr);

#define __kernel_cmpxchg (*(__kernel_cmpxchg_t *)0xffff0fc0)

static inline size_t __CAS(size_t* ptr, size_t oldval, size_t newval) {
  while (1) {
    int fail;
    size_t cur=*ptr;
    if (__builtin_expect(oldval != cur, 0))
      return cur;
    fail=__kernel_cmpxchg(cur,newval,ptr);
    if (__builtin_expect(!fail, 1))
      return oldval;
  }
}

#else

/* This function does this, atomically:

   if (*ptr == oldval) {
     *ptr=newval;
     return oldval;
   } else
     return *ptr;

  It can be used to implement lock-free data structures or locking
  primitives.
*/

size_t __CAS(size_t* ptr,size_t oldval,size_t newval);

#if (defined(__sparc__) && !defined(__arch64__)) || defined(__hppa__)
#define NO_CAS
#endif

#endif

#if defined(__INTEL_COMPILER) || (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1))

#define __atomic_add(ptr,val) __sync_fetch_and_add(ptr,val)

#else

static inline size_t __atomic_add(size_t* ptr,size_t val) {
  size_t r,o;
  do {
    r=__CAS(ptr,(o=*ptr),*ptr+val);
  } while (r!=o);
  return r;
}

#endif

#endif
