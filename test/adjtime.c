#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/time.h>

int main() {
  struct timeval a,b;
  int rc;
  a.tv_sec=0; a.tv_usec=0;
  rc = adjtime(&a,&b);
  assert(!rc || errno == EPERM);

  rc = adjtime(&a, NULL);
  assert(!rc || errno == EPERM);

  rc = adjtime(NULL,&b);
  assert(!rc);
  assert(b.tv_sec  < 30);	    /* 30 seconds delta is very unlikely... */
  assert(b.tv_sec  > -30);	    /* 30 seconds delta is very unlikely... */
  assert(b.tv_usec > -1000000);
  assert(b.tv_usec < 1000000);

  printf("%lu/%d\n", (unsigned long)b.tv_sec, (int)b.tv_usec);
  return 0;
}
