#include <stdio.h>
#include <pthread.h>

#include <unistd.h>
#include <signal.h>

#include <errno.h>

int ra;

void test(int sig)
{
  printf("SIGNAL %d in %d\n",sig,getpid());
  fflush(stdout);
  signal(SIGHUP, test);
}

void* thread(void* arg)
{
  int i;
  for (i=0;i<10; i++)
  {
    printf("child %ld %8p\n", pthread_self(),arg);
    sleep(1);
  }
  return arg;
}

void* thread2(void *arg)
{
  pthread_t t;
  int i;

  printf("child %ld %8p\n", pthread_self(),arg);
  for (i=0; i<3; ++i) {
    printf("child starting next in %d secs %ld %8p\n", 3-i, pthread_self(),arg);
    sleep(1);
  }
  pthread_create(&t,0, thread, (void*)2);
  printf("new child @ %ld\n",t);
  return (void*)t;
}

int main(int argc, char*argv[])
{
  pthread_t t;

  signal(SIGHUP, test);

  pthread_create(&t,0, thread2, 0);
  pthread_create(&t,0, thread, (void *)3);
  pthread_create(&t,0, thread, (void *)1);

  puts("main");

#if 0
  {
    int i;
    for(i=0;i<4;i++) sleep(1);
  }
#else
  {
    int *tt;
    int i = pthread_join(t,(void*)&tt);
    printf("join %d %p %d\n",i,tt,(*(__errno_location())));
  }
#endif

  puts("main exit");

  return 0;
}
