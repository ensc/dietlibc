#ifndef _GRP_H
#define _GRP_H

#include <sys/cdefs.h>
#include <sys/types.h>

struct group
  {
    char *gr_name;		/* Group name.	*/
    char *gr_passwd;		/* Password.	*/
    gid_t gr_gid;		/* Group ID.	*/
    char **gr_mem;		/* Member list.	*/
  };

extern struct group *getgrgid (gid_t uid) __THROW;
extern struct group *getgrnam (const char *name) __THROW;

extern struct group *getgrent(void) __THROW;
extern void setgrent(void) __THROW;
extern void endgrent(void) __THROW;

int getgrent_r(struct group *res, char *buf, size_t buflen,
	       struct group **res_sig) __THROW;
int getgrnam_r(const char* name,
	       struct group *res, char *buf, size_t buflen,
	       struct group **res_sig) __THROW;
int getgrgid_r(uid_t uid,
	       struct group *res, char *buf, size_t buflen,
	       struct group **res_sig) __THROW;

extern int setgroups(size_t n, const gid_t *groups) __THROW;
extern int setgroups32(size_t n, const gid32_t *groups) __THROW;
extern int initgroups(const char *user, gid_t group) __THROW;

#endif
