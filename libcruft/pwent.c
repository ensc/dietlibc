/*
 * dietlibc/lib/pwent.c - getpwent(3)/setpwent(3)/endpwent(3) implementation
 *
 * Copyright 2001 Jeff Garzik <jgarzik@mandrakesoft.com>
 *
 * This is a brand new implementation, based on the interface
 * described in man-pages-1.34-1mdk man pages package.
 */


#include "entlib.h" /* this is our only include */

static struct __ent_state *st = NULL; /* state held across calls */
static struct passwd pw;

struct passwd *getpwent(void)
{
	char *parts[7];

	if (__ent_start(_PATH_PASSWD, &st))
		return NULL;
	if (__ent_get_line(st))
		goto err_out;
	if (__ent_split(st, parts, 7, ':', 1) < 0)
		goto err_out;

	pw.pw_name	= parts[0];
	pw.pw_passwd	= parts[1];
	pw.pw_uid	= (uid_t) atoi(parts[2]);
	pw.pw_gid	= (gid_t) atoi(parts[3]);
	pw.pw_gecos	= parts[4];
	pw.pw_dir	= parts[5];
	pw.pw_shell	= parts[6];

	return &pw;

err_out:
	st->buflen = st->bufptr = 0;	/* invalidate file data buffer */
	return NULL;
}

void setpwent(void) { __ent_set(st); }
void endpwent(void) { __ent_end(st); st=NULL; }
