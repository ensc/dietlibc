#ifndef _ERRNO_H
#define	_ERRNO_H

#ifndef _REENTRANT
extern int errno;
#else
#define errno (*__errno_location())
#endif

#include <linux/errno.h>

extern int *__errno_location(void);

#define __set_errno(x) errno=(x)

#endif
