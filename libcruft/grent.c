/*
 * dietlibc/lib/grent.c - getgrent(3)/setgrent(3)/endgrent(3) implementation
 *
 * Copyright 2001 Jeff Garzik <jgarzik@mandrakesoft.com>
 *
 * This is a brand new implementation, based on the interface
 * described in man-pages-1.34-1mdk man pages package.
 */


#include "entlib.h" /* this is our only include */

#define MAX_GROUP_MEMBERS 16 /* matches Linux kernel task struct */

static struct __ent_state *st = NULL; /* state held across calls */
static char *gr_mem[MAX_GROUP_MEMBERS+1];
static struct group gr;

struct group *getgrent(void)
{
	char *parts[4], *grouplist;
	unsigned int bufptr;

	if (__ent_start(_PATH_GROUP, &st))
		return NULL;
	if (__ent_get_line(st))
		goto err_out;
	if (__ent_split(st, parts, 4, ':', 1) < 0)
		goto err_out;

	gr.gr_name	= parts[0];
	gr.gr_passwd	= parts[1];
	gr.gr_gid	= (gid_t) atoi(parts[2]);
	grouplist	= parts[3];

	memset(&gr_mem, 0, sizeof(gr_mem));
	gr.gr_mem = gr_mem;

	/* rewind bufptr to beginning of group list */
	bufptr = st->bufptr;
	st->bufptr = (int) (grouplist - st->ent_buf);

	__ent_split(st, gr_mem, MAX_GROUP_MEMBERS, ',', 0);
	st->bufptr = bufptr;

	return &gr;

err_out:
	st->buflen = st->bufptr = 0;	/* invalidate file data buffer */
	return NULL;
}

void setgrent(void) { __ent_set(st); }
void endgrent(void) { __ent_end(st); st=NULL; }
