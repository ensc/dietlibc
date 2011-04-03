/*
 * nice() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codepoet.org>
 * Copyright (C) 2005 by Manuel Novoa III <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include <errno.h>
#include <limits.h>
#include <sys/resource.h>

static inline int int_add_no_wrap(int a, int b)
{
	int	s;

	if ((b > 0) && (a > (INT_MAX - b)))
		s = INT_MAX;
	else if ((b < 0) && (a < (INT_MIN - b)))
		s = INT_MIN;
	else
		s = a + b;

	return s;
}

static inline int __syscall_nice(int incr)
{
	int old_priority;
	int old_errno;

	old_errno = errno;
	__set_errno(0);
	old_priority = getpriority(PRIO_PROCESS, 0);
	if ((old_priority == -1) && errno) {
		return -1;
	}
	__set_errno(old_errno);

	if (setpriority(PRIO_PROCESS, 0, int_add_no_wrap(old_priority, incr))) {
		__set_errno(EPERM);	/* SUSv3 mandates EPERM for nice failure. */
		return -1;
	}

	return 0;
}

int nice(int incr)
{
	if (__syscall_nice(incr)) {
		return -1;
	}

	return getpriority(PRIO_PROCESS, 0);
}
