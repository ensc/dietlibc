#define _BSD_SOURCE
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
//    printf("%02x%c",(unsigned char)buf[i],i%16==15?'\n':i%8==7?' ':':');
    if (buf[i]==0)
      ++j;
    else if (j>=100) {
//      putchar('\n');
      return 0;
    }
  }
//  putchar('\n');
  return 1;
}

char a[16*1024];
char b[16*1024];

int test_bcmp() {
  size_t i;
  struct timespec x,y;
  long diff[4];

  memset(a,23,sizeof(a));
  memset(b,23,sizeof(b));
  assert(clock_gettime(CLOCK_MONOTONIC,&x)==0);
  for (i=0; i<100000; ++i) {
    assert(timingsafe_bcmp(a,b,sizeof(a))==0);
  }
  assert(clock_gettime(CLOCK_MONOTONIC,&y)==0);
  diff[0]=(y.tv_sec-x.tv_sec)*1000000000 + y.tv_nsec-x.tv_nsec;
  printf("timingsafe_bcmp: %lu.%09lu %lu.%09lu - %lu\n",x.tv_sec,x.tv_nsec,y.tv_sec,y.tv_nsec,diff[0]);

  assert(clock_gettime(CLOCK_MONOTONIC,&x)==0);
  for (i=0; i<100000; ++i) {
    assert(bcmp(a,b,sizeof(a))==0);
  }
  assert(clock_gettime(CLOCK_MONOTONIC,&y)==0);
  diff[1]=(y.tv_sec-x.tv_sec)*1000000000 + y.tv_nsec-x.tv_nsec;
  printf("   regular bcmp: %lu.%09lu %lu.%09lu - %lu\n",x.tv_sec,x.tv_nsec,y.tv_sec,y.tv_nsec,diff[1]);

  a[10]=24;	// now give bcmp an opportunity to exit early

  assert(clock_gettime(CLOCK_MONOTONIC,&x)==0);
  for (i=0; i<100000; ++i) {
    assert(timingsafe_bcmp(a,b,sizeof(a))==1);
  }
  assert(clock_gettime(CLOCK_MONOTONIC,&y)==0);
  diff[2]=(y.tv_sec-x.tv_sec)*1000000000 + y.tv_nsec-x.tv_nsec;
  printf("timingsafe_bcmp: %lu.%09lu %lu.%09lu - %lu\n",x.tv_sec,x.tv_nsec,y.tv_sec,y.tv_nsec,diff[2]);

  assert(clock_gettime(CLOCK_MONOTONIC,&x)==0);
  for (i=0; i<100000; ++i) {
    assert(bcmp(a,b,sizeof(a))==1);
  }
  assert(clock_gettime(CLOCK_MONOTONIC,&y)==0);
  diff[3]=(y.tv_sec-x.tv_sec)*1000000000 + y.tv_nsec-x.tv_nsec;
  printf("   regular bcmp: %lu.%09lu %lu.%09lu - %lu\n",x.tv_sec,x.tv_nsec,y.tv_sec,y.tv_nsec,diff[3]);

  /* we expect the timingsafe_bcmp values to be roughly the same (+- 10% for measurement inaccuracies) */
  long delta=diff[0]-diff[2];
  if (delta<0) delta=-delta;
  assert(delta*10<diff[0]);
}

int test_memcmp() {
  size_t i;
  struct timespec x,y;
  long diff[4];

  memset(a,23,sizeof(a));
  memset(b,23,sizeof(b));
  assert(clock_gettime(CLOCK_MONOTONIC,&x)==0);
  for (i=0; i<100000; ++i) {
    assert(timingsafe_memcmp(a,b,sizeof(a))==0);
  }
  assert(clock_gettime(CLOCK_MONOTONIC,&y)==0);
  diff[0]=(y.tv_sec-x.tv_sec)*1000000000 + y.tv_nsec-x.tv_nsec;
  printf("timingsafe_memcmp: %lu.%09lu %lu.%09lu - %lu\n",x.tv_sec,x.tv_nsec,y.tv_sec,y.tv_nsec,diff[0]);

  assert(clock_gettime(CLOCK_MONOTONIC,&x)==0);
  for (i=0; i<100000; ++i) {
    assert(memcmp(a,b,sizeof(a))==0);
  }
  assert(clock_gettime(CLOCK_MONOTONIC,&y)==0);
  diff[1]=(y.tv_sec-x.tv_sec)*1000000000 + y.tv_nsec-x.tv_nsec;
  printf("   regular memcmp: %lu.%09lu %lu.%09lu - %lu\n",x.tv_sec,x.tv_nsec,y.tv_sec,y.tv_nsec,diff[1]);

  a[10]=24;	// now give memcmp an opportunity to exit early

  assert(clock_gettime(CLOCK_MONOTONIC,&x)==0);
  for (i=0; i<100000; ++i) {
    assert(timingsafe_memcmp(a,b,sizeof(a))==1);
  }
  assert(clock_gettime(CLOCK_MONOTONIC,&y)==0);
  diff[2]=(y.tv_sec-x.tv_sec)*1000000000 + y.tv_nsec-x.tv_nsec;
  printf("timingsafe_memcmp: %lu.%09lu %lu.%09lu - %lu\n",x.tv_sec,x.tv_nsec,y.tv_sec,y.tv_nsec,diff[2]);

  assert(clock_gettime(CLOCK_MONOTONIC,&x)==0);
  for (i=0; i<100000; ++i) {
    assert(memcmp(a,b,sizeof(a))==1);
  }
  assert(clock_gettime(CLOCK_MONOTONIC,&y)==0);
  diff[3]=(y.tv_sec-x.tv_sec)*1000000000 + y.tv_nsec-x.tv_nsec;
  printf("   regular memcmp: %lu.%09lu %lu.%09lu - %lu\n",x.tv_sec,x.tv_nsec,y.tv_sec,y.tv_nsec,diff[3]);

  /* we expect the timingsafe_memcmp values to be roughly the same (+- 10% for measurement inaccuracies) */
  long delta=diff[0]-diff[2];
  if (delta<0) delta=-delta;
  assert(delta*10<diff[0]);
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
  assert(check()==0);
  test_bcmp();
  test_memcmp();
  return 0;
}
