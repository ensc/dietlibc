#ifndef _IOCTL_H
#define _IOCTL_H

#include <sys/cdefs.h>
#include <linux/ioctl.h>
#include <asm/ioctls.h>

int ioctl(int d, int request, ...) __THROW;

#endif
