#ifndef	_TERMIOS_H
#define	_TERMIOS_H

#include <sys/cdefs.h>
#include <sys/types.h>

/* man, the Linux kernel headers suck */
#define __USE_BSD
#include <linux/termios.h>
#undef __USE_BSD

pid_t tcgetpgrp(int fd) __THROW;
int tcsetpgrp(int fd, pid_t pgrpid) __THROW;
int tcgetattr(int fd, struct termios *termios_p) __THROW;
int tcsetattr(int fd, int optional_actions, struct termios *termios_p) __THROW;
speed_t cfgetospeed(struct termios *termios_p) __THROW;
int cfsetospeed(struct termios *termios_p, speed_t speed) __THROW;
speed_t cfgetispeed(struct termios *termios_p) __THROW;
int cfsetispeed(struct termios *termios_p, speed_t speed) __THROW;

int tcflush(int fd, int queue_selector) __THROW;
int tcdrain(int fd) __THROW;

#endif
