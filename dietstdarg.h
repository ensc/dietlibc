#include <stdarg.h>

/* this assumes that va_copy() will be a macro, it is on gcc */
#ifndef va_copy
# ifdef __va_copy
#  define va_copy(x, y) __va_copy(x, y)
# else
/* assume copying it works... */
#  define va_copy(x, y) x = y
# endif
#endif

