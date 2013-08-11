#define _BSD_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include "../dietdirent.h"

static int rc = 0;

static void xputs(char const *str)
{
  fputs(str, stderr);
  rc = 1;
}

static int create_file(char const *dir, char const *name)
{
  char	buf[strlen(dir) + strlen(name) + sizeof("/")];
  strcpy(buf, dir);
  strcat(buf, "/");
  strcat(buf, name);

  return close(open(buf, O_WRONLY|O_CREAT,0666));
}

static int remove_file(char const *dir, char const *name)
{
  char	buf[strlen(dir) + strlen(name) + sizeof("/")];
  strcpy(buf, dir);
  strcat(buf, "/");
  strcat(buf, name);

  return unlink(buf);
}

int main() {
  char tmpdir[] = "/tmp/readdir.XXXXXX";
  unsigned long res = 0;
  DIR* D;
  if (!mkdtemp(tmpdir)) {
    perror("mkdtemp");
    return 1;
  }
  D=opendir(tmpdir);
  if (!D) {
    perror("opendir");
    rmdir(tmpdir);
    return 1;
  }

  if (chdir(tmpdir) < 0) {
    perror("chdir");
    rmdir(tmpdir);
    return 1;
  }
  close(open("bar",O_WRONLY|O_CREAT,0666));
  close(open("baz",O_WRONLY|O_CREAT,0666));

  struct dirent* d;

  while ((d=readdir(D))) {
    struct stat		st;

    if (strcmp(d->d_name,".") == 0)
      res |= 1<<0;
    else if (strcmp(d->d_name,"..") == 0)
      res |= 1<<1;
    else if (strcmp(d->d_name,"bar") == 0)
      res |= 1<<2;
    else if (strcmp(d->d_name,"baz") == 0)
      res |= 1<<3;
    else
      xputs("got additional file");

    if (lstat(d->d_name, &st) < 0) {
      perror("lstat");
      rc = 1;
    } else if (st.st_ino != d->d_ino) {
      xputs("mismatch in ino");
    } else if (d->d_type == DT_UNKNOWN) {
      /* noop */
    } else if ((d->d_type == DT_REG && !S_ISREG(st.st_mode)) ||
	       (d->d_type == DT_DIR && !S_ISDIR(st.st_mode))) {
      xputs("mismatch in d_type");
    } else if (d->d_type != DT_REG && d->d_type != DT_DIR) {
      xputs("unexpected d_type");
    }

    if (telldir(D) != d->d_off) {
      xputs("mismatch with telldir");
    }

    unlink(d->d_name);
  }

  if (res != 0x0f)
    xputs("did not got all expected files");

  rmdir(tmpdir);
  return rc;
}
