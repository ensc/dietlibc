#include <sys/types.h>
#include <string.h>

int  strncasecmp ( const char* s1, const char* s2, size_t len )
{
    register unsigned int  x2;
    register unsigned int  x1;
    register const char*   end = s1 + len;

    while (1) {
        if ( s1 >= end )
            return 0;
        x2 = *s2++ - 'A'; if (x2 < 26u) x2 += 32;
        x1 = *s1++ - 'A'; if (x1 < 26u) x1 += 32;
        if ( x2 != x1 )
            break;
        if ( x1 == (unsigned int)-'A' )
            break;
    }

    return x1 - x2;
}
