#include <ctype.h>

int __iscntrl_ascii ( int ch ) 
{
    return ch < 32u  ||  ch == 127;
}

int iscntrl ( int ch ) __attribute__((weak,alias("__iscntrl_ascii")));
