#include <termios.h>
#include <sys/ioctl.h>

int tcgetpgrp(int fildes)
{
  return ioctl(fildes, TIOCGPGRP);
}
