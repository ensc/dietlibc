#include <termios.h>
#include <sys/types.h>

speed_t cfgetospeed(struct termios *termios_p) {
  return termios_p->c_ospeed;
}
