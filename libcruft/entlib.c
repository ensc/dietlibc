/*
 * dietlibc/lib/entlib.c - Generic delimited-line parsing library
 *
 * Copyright 2001 Jeff Garzik <jgarzik@mandrakesoft.com>
 *
 * This is a brand new implementation, based on the interface
 * described in man-pages-1.34-1mdk man pages package.
 */


#include <errno.h>
#include "dietfeatures.h"
#include "entlib.h" /* this is our only include */


/*
 * __ent_start
 *
 * Allocates and zeroes the module's state structure,
 * and open a handle to /etc/passwd.
 *
 * Returns -1 on failure and sets errno, or zero for success.
 */

int __ent_start(const char *pathname, struct __ent_state **st_ref)
{
	struct __ent_state *st;
	
	if (*st_ref)
		return 0;

	st = calloc(1, sizeof(*st));
	if (!st) {
		errno = ENOMEM;
		return -1;
	}

	st->fd = open(pathname, O_RDONLY);
	if (st->fd == -1) {
		/* errno should have been set by open(2) */
		free(st);
		st = NULL;
		return -1;
	}
	fcntl (st->fd, F_SETFD, FD_CLOEXEC);	/* if it fails: too bad */

	*st_ref = st;
	return 0;
}


/*
 * __ent_get_line
 *
 * Eliminates a previous line from the buffer, if any.
 * Then reads in a new line from /etc/passwd, if necessary.
 *
 * Returns -1 on failure, or zero for success.
 */

int __ent_get_line(struct __ent_state *st)
{
	int rc;

	/* overwrite previous line, by shifting the rest
	 * of the rest to the front of the buffer
	 */
	if (st->bufptr) {
		unsigned int slop = st->buflen - st->bufptr;
		memmove(st->ent_buf, &st->ent_buf[st->bufptr], slop);
		st->bufptr = 0;
		st->buflen = slop;
		st->ent_buf[st->buflen] = 0; /* null terminate */
	}

	if (st->buflen == __ENT_BUFSIZ || strchr(st->ent_buf, '\n'))
		return 0;

	rc = read(st->fd, &st->ent_buf[st->buflen], __ENT_BUFSIZ - st->buflen);
	if (rc < 0)
		return -1;

	st->buflen += rc;
	if (st->buflen == 0)
		return -1;

	return 0;
}


/*
 * __ent_split
 *
 * Splits a string into parts based on a delimiter.
 * Stops processing when \n is reached also.
 *
 * Returns -1 on failure, or zero on success.
 */

int __ent_split(struct __ent_state *st, char **parts,
		int n_parts, int delimiter, int require_exact)
{
	char *s = &st->ent_buf[st->bufptr];
	int idx = 0;

	/* empty list */
	if (!*s) {
		if (!require_exact)
			return 0;
		return -1;
	}

	/* scan through string, sticking string pointers
	 * into parts[] as delimiters are found
	 */
	parts[idx++] = s;
	while (*s) {
		st->bufptr++;
		if (*s == '\n') {
			*s = 0; /* null terminate */
			break;
		}
		if (*s == delimiter) {
			*s = 0; /* null terminate */
			/* boundary error: too many delimiters */
			if (idx == n_parts)
				return -1;
			s++;
			parts[idx++] = s;
		} else {
			s++;
		}
	}

	if (!require_exact)
		return 0;
	return (n_parts == idx) ? 0 : -1;
}


void __ent_set(struct __ent_state *st)
{
	if (!st)
		return;
	st->buflen = st->bufptr = 0;
	lseek(st->fd, 0, SEEK_SET);
}


void __ent_end(struct __ent_state *st)
{
	if (!st)
		return;
	close(st->fd);
	free(st);
}
