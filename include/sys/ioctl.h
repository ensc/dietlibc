#ifndef _IOCTL_H
#define _IOCTL_H

#include <sys/cdefs.h>

#if defined(__i386__)
#include <sys/i386-ioctl.h>
#elif defined(__alpha__)
#include <sys/alpha-ioctl.h>
#elif defined(__arm__)
#include <sys/arm-ioctl.h>
#elif defined(__sparc__)
#include <sys/sparc-ioctl.h>
#elif defined(__mips__)
#include <sys/mips-ioctl.h>
#elif defined(__powerpc__)
#include <sys/ppc-ioctl.h>
#elif defined(__s390__)
#include <sys/s390-ioctl.h>
#elif defined(__hppa__)
#include <sys/hppa-ioctl.h>
#endif

int ioctl(int d, int request, ...) __THROW;

#endif
