#ifndef _SHADOW_H
#define _SHADOW_H

#include <sys/cdefs.h>
#include <sys/types.h>

struct spwd {
	char *sp_namp;		/* user login name */
	char *sp_pwdp;		/* encrypted password */
	long  sp_lstchg;	/* last password change */
	int   sp_min;		/* days until change allowed. */
	int   sp_max;		/* days before change required */
	int   sp_warn;		/* days warning for expiration */
	int   sp_inact;		/* days before  account  inactive */
	int   sp_expire;	/* date when account expires */
	int   sp_flag;		/* reserved for future use */
};

extern struct spwd *getspent(void) __THROW;
extern void setspent(void) __THROW;
extern void endspent(void) __THROW;
extern struct spwd *getspnam (__const char *__name) __THROW;

#endif /* _SHADOW_H */
