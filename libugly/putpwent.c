/* man, what a crook! */

#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>

int putpwent(const struct passwd *p, FILE *stream) {
  fprintf(stream,"%s:%s:%d:%d:%s:%s:%s\n", p->pw_name, p->pw_passwd,
	  p->pw_uid, p->pw_gid, p->pw_gecos, p->pw_dir, p->pw_shell);
}
