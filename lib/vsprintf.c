#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include "dietwarning.h"

int vsnprintf (char *str,size_t size,const char *format, va_list arg_ptr);

int vsprintf(char *dest,const char *format, va_list arg_ptr)
{
  return vsnprintf(dest,-1,format,arg_ptr);
}

link_warning("vsprintf","warning: Avoid *sprintf; use *snprintf. It is more secure.")
