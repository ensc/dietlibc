#include "dietfeatures.h"
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>

int tcflow (int fd,int action)
{
   switch (action)
     {
      case TCOOFF:
        return (ioctl (fd,TCXONC,0));
      case TCOON:
        return (ioctl (fd,TCXONC,1));
      case TCIOFF:
        return (ioctl (fd,TCXONC,2));
      case TCION:
        return (ioctl (fd,TCXONC,3));
      default:
        errno = EINVAL;
        return (-1);
     }
}
