#ifndef _PWD_H
#define _PWD_H

#include <sys/cdefs.h>
#include <sys/types.h>

struct passwd {
  char *pw_name;		/* Username.  */
  char *pw_passwd;		/* Password.  */
  uid_t pw_uid;			/* User ID.  */
  gid_t pw_gid;			/* Group ID.  */
  char *pw_gecos;		/* Real name.  */
  char *pw_dir;			/* Home directory.  */
  char *pw_shell;		/* Shell program.  */
};

extern struct passwd *getpwuid (uid_t __uid) __THROW;
extern struct passwd *getpwnam (const char *__name) __THROW;

extern struct passwd *getpwent(void) __THROW;
extern void setpwent(void) __THROW;
extern void endpwent(void) __THROW;
int putpwent(const struct passwd *p, FILE *stream) __THROW;


#endif
