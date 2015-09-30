#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void doit(int fd) {
  char buf[100];
  int i;
  for (i=0; i<100; ++i) buf[i]=i;	/* create "secret" */
  write(fd,buf,100);
  close(fd);
//  memset(buf,0,sizeof(buf));
  explicit_bzero(buf,100);
}

int check() {
  char buf[200];	/* purposefully not initialized so it hopefully contains the data we left behind in doit */
  int i,j;
  for (i=j=0; i<200; ++i) {
    printf("%02x%c",(unsigned char)buf[i],i%16==15?'\n':i%8==7?' ':':');
    if (buf[i]==0)
      ++j;
    else if (j>=100) {
      putchar('\n');
      return 0;
    }
  }
  putchar('\n');
  return 1;
}

int main() {
  char template[]="/tmp/dummyXXXXXX";
  int fd;
  char* c;
  size_t i;
  assert(fd=mkstemp(template));
  unlink(template);
  doit(fd);
  /* now insist we find 100 0 bytes on the stack */
  return check();
}
