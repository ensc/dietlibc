#include "dietfeatures.h"
#include <unistd.h>
#include <string.h>

#define _BSD_SOURCE
#include <errno.h>

#define __NO_CODE
#include "errlist.c"

extern const char  __sys_err_unknown [];

const char*  strerror ( int errnum )
{
    register const char*  message = __sys_err_unknown;

    if ( (unsigned int)errnum < (unsigned int)__SYS_NERR )
        message = sys_errlist [errnum];
    return message;
}
