#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include "dietwarning.h"

int vsprintf(char *dest,const char *format, va_list arg_ptr)
{
  return vsnprintf(dest,0,format,arg_ptr);
}

link_warning("vsprintf","warning: Avoid *sprintf; use *snprintf. It is more secure.")
