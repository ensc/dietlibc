#include "dietfeatures.h"
#include <errno.h>

#ifndef WANT_THREAD_SAFE
extern int errno;
#endif

void __set_errno(int error) __attribute__ ((weak));

void __set_errno(int error)
{
#ifdef WANT_THREAD_SAFE
  (*__errno_location())=error;
#else
  errno=error;
#endif
}

