#include "dietfeatures.h"
#include <errno.h>

extern int errno;

void __set_errno(int error) __attribute__ ((weak));

void __set_errno(int error)
{
  errno=error;
}

