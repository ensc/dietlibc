#include <termios.h>
#include <sys/ioctl.h>

int tcsetpgrp(int fildes, pid_t pgrpid)
{
  return ioctl(fildes, TIOCSPGRP, &pgrpid);
}
