#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

/* qmailr:x:34:34:qmail remote user:/var/qmail:/bin/sh */

static char *parse(char *map, struct passwd *p, char *eof) {
  char *tmp;
  int i;
  if (map+12 > eof) return eof;

  tmp=map;

  for (i=0; i<7; i++) {
    switch (i) {
    case 0: p->pw_name=tmp; break;
    case 1: p->pw_passwd=tmp; break;
    case 2: p->pw_uid=atoi(tmp); break;
    case 3: p->pw_gid=atoi(tmp); break;
    case 4: p->pw_gecos=tmp; break;
    case 5: p->pw_dir=tmp; break;
    case 6: p->pw_shell=tmp; break;
    }
    for (; *tmp&&(*tmp!='\n')&&(*tmp!=':'); tmp++);
    if (i<6) {
      if (*tmp!=':') return eof;
      *tmp=0;
    } else {
      if (*tmp==':') return eof;
    }
    tmp++;
  }
  return tmp;
}

#if 0
struct passwd
{
  char *pw_name;		/* Username.  */
  char *pw_passwd;		/* Password.  */
  __uid_t pw_uid;		/* User ID.  */
  __gid_t pw_gid;		/* Group ID.  */
  char *pw_gecos;		/* Real name.  */
  char *pw_dir;			/* Home directory.  */
  char *pw_shell;		/* Shell program.  */
};
#endif

static struct passwd *doit(int mode,const char*name,uid_t uid) {
  int fd=open("/etc/passwd",O_RDONLY);
  char *map=(char*)-1;
  off_t len=0;
  struct passwd *foo;
  char *tmp;
  int found=0;

  if (fd<0) goto error;
  len=lseek(fd,0,SEEK_END);
  foo=(struct passwd*)malloc(sizeof(struct passwd));
  if (!foo) goto error;
  map=mmap(0,len,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
  if ((long)map==(-1)) goto error;

  foo->pw_uid=uid+1;
  for (tmp=map; tmp<=map+len; tmp=parse(tmp,foo,map+len)) {
    if (mode) {
      if (foo->pw_uid == uid) {
	found=1;
	break;
      }
    } else {
      if (foo->pw_name && !strcmp(foo->pw_name,name)) {
	found=1;
	break;
      }
    }
  }
  if (found) { /* found */
    foo->pw_name=strdup(foo->pw_name);
    foo->pw_passwd=strdup(foo->pw_passwd);
    foo->pw_gecos=strdup(foo->pw_gecos);
    foo->pw_dir=strdup(foo->pw_dir);
    foo->pw_shell=strdup(foo->pw_shell);
  } else {
    free(foo);
    foo=0;
  }

  munmap(map,len);
  close(fd);
  return foo;
error:
  if (map!=(char*)-1) munmap(map,len);
  if (fd!=-1) close(fd);
  errno=ENOMEM;
  return 0;
}

struct passwd *getpwnam(const char * name) {
  return doit(0,name,0);
}

struct passwd *getpwuid(uid_t uid) {
  return doit(1,0,uid);
}
