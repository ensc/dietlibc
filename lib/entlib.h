/*
 * dietlibc/lib/entlib.h - Generic delimited-line parsing library header
 *
 * Copyright 2001 Jeff Garzik <jgarzik@mandrakesoft.com>
 *
 * This is a brand new implementation, based on the interface
 * described in man-pages-1.34-1mdk man pages package.
 */

#ifndef __ENTLIB_H
#define __ENTLIB_H

#include <sys/types.h>	/* for open(2), lseek(2) */
#include <sys/stat.h>	/* for open(2) */
#include <fcntl.h>	/* for open(2) */
#include <unistd.h>	/* for close(2), lseek(2) */
#include <stdlib.h>	/* for calloc(3), free(3) */
#include <errno.h>	/* for errno */
#include <string.h>	/* for memmove(3) */
#include <stdlib.h>	/* for atoi(3) */

#include <pwd.h>	/* for struct passwd */
#include <grp.h>	/* for struct group */
#include <shadow.h>	/* for struct spwd */

/* should be a multiple of 64 to create decent alignment */
#define __ENT_BUFSIZ (64 * 2)
#define __ENT_RAW_BUFSIZ (__ENT_BUFSIZ + 2) /* some overlap required */

struct __ent_state {
	char ent_buf[__ENT_RAW_BUFSIZ];	/* raw file data buffer */
	unsigned int buflen;		/* amount of valid data in ent_buf */
	unsigned int bufptr;		/* next valid position in ent_buf */
	int fd;				/* /etc/passwd file descriptor */
};

extern int __ent_start(const char *pathname, struct __ent_state **st_ref);
extern int __ent_get_line(struct __ent_state *st);
extern int __ent_split(struct __ent_state *st, char **parts, int n_parts, int delimiter, int require_exact);
extern void __ent_set(struct __ent_state *st);
extern void __ent_end(struct __ent_state *st);

#endif /* __ENTLIB_H */
