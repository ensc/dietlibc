#define _FILE_OFFSET_BITS 64
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main() {
  DIR* D;
  mkdir("/tmp/foo",0777);
  close(open("/tmp/foo/bar",O_WRONLY|O_CREAT,0666));
  close(open("/tmp/foo/baz",O_WRONLY|O_CREAT,0666));
  D=opendir("/tmp/foo");
  struct dirent* d;
  if (!D) {
    perror("opendir");
    return 1;
  }
  if (!(d=readdir(D)) || strcmp(d->d_name,".")) puts("did not get \".\"");
  if (!(d=readdir(D)) || strcmp(d->d_name,"..")) puts("did not get \"..\"");
  if (!(d=readdir(D)) || strcmp(d->d_name,"bar")) puts("did not get \"bar\"");
  if (!(d=readdir(D)) || strcmp(d->d_name,"baz")) puts("did not get \"baz\"");
  if ((d=readdir(D))) puts("got additional file");
#if 0
  while ((d=readdir(D))) {
    printf("found %s\n",d->d_name);
  }
#endif
  unlink("/tmp/foo/bar");
  unlink("/tmp/foo/baz");
  rmdir("/tmp/foo");
  return 0;
}
