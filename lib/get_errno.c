#include "dietfeatures.h"
#include <errno.h>

extern int errno;

int __get_errno() __attribute__ ((weak));

int __get_errno()
{
  return errno;
}

