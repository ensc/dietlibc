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

extern struct group *getgrgid (uid_t __uid) __THROW;
extern struct group *getgrnam (const char *__name) __THROW;

extern struct group *getgrent(void) __THROW;
extern void setgrent(void) __THROW;
extern void endgrent(void) __THROW;

#endif
