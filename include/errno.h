#ifndef _ERRNO_H
#define	_ERRNO_H

extern int errno;

#include <linux/errno.h>

extern int *__errno_location();
extern void __set_errno(int error);

#endif
