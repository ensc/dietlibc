#include <termios.h>
#include <errno.h>

#define IBAUD0  020000000000

int cfsetispeed(struct termios *termios_p, speed_t speed)
{
  if ((speed & ~CBAUD) != 0 && (speed < B57600 || speed > B460800)) {
    __set_errno(EINVAL);
    return -1;
  }
  if (speed == 0)
    termios_p->c_iflag |= IBAUD0;
  else {
    termios_p->c_iflag &= ~IBAUD0;
    termios_p->c_cflag &= ~(CBAUD | CBAUDEX);
    termios_p->c_cflag |= speed;
  }
  return 0;
}

