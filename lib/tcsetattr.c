#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "dietfeatures.h"

int  tcsetattr ( int fildes, int optional_actions, struct termios* termios_p )
{
#if TCSANOW==0 && TCSADRAIN==1 && TCSAFLUSH==2 && TCSETSW-TCSETS==1 && TCSETSF-TCSETS==2

    if ( (unsigned int)optional_actions < 3u )
        return ioctl ( fildes, TCSETS+optional_actions, termios_p );

    errno = EINVAL;
    return -1;

#else

    switch ( optional_actions ) {
    case TCSANOW:
        return ioctl ( fildes, TCSETS , termios_p );
    case TCSADRAIN:
        return ioctl ( fildes, TCSETSW, termios_p );
    case TCSAFLUSH:
        return ioctl ( fildes, TCSETSF, termios_p );
    default:
        errno = EINVAL;
        return -1;
    }
    
#endif    
}
