#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

#define PTHREAD_NUM_ATFORK 4

static struct _pthread_fastlock __atfork_struct_lock;
static struct {
  int used;
  void (*prepare)(void);
  void (*parent)(void);
  void (*child)(void);
} pthread_atfork_buf[PTHREAD_NUM_ATFORK];

int pthread_atfork(void (*prepare)(void),
		   void (*parent)(void),
		   void (*child)(void))
{
  int i,ret=-1;

  __NO_ASYNC_CANCEL_BEGIN;
  __pthread_lock(&__atfork_struct_lock);

  for (i=0;i<PTHREAD_NUM_ATFORK; ++i) {
    if (!(pthread_atfork_buf[i].used)) {
      pthread_atfork_buf[i].prepare=prepare;
      pthread_atfork_buf[i].parent=parent;
      pthread_atfork_buf[i].child=child;
      ret=0;
      break;
    }
  }

  __pthread_unlock(&__atfork_struct_lock);
  __NO_ASYNC_CANCEL_END;

  if (ret) ret=ENOMEM;
  return ret;
}

pid_t fork(void)
{
  pid_t pid;
  int i=PTHREAD_NUM_ATFORK;

  __NO_ASYNC_CANCEL_BEGIN;

  __TEST_CANCEL();
  __pthread_lock(&__atfork_struct_lock);

  while (i) {
    if (pthread_atfork_buf[--i].used)
      if (pthread_atfork_buf[i].prepare)
	pthread_atfork_buf[i].prepare();
  }

  pid=__libc_fork();

  if (pid>0) {		/* parent */
    for (i=0;i<PTHREAD_NUM_ATFORK;++i)
      if (pthread_atfork_buf[i].used)
	if (pthread_atfork_buf[i].parent)
	  pthread_atfork_buf[i].parent();
  } else if (pid==0) {	/* child */
    for (i=0;i<PTHREAD_NUM_ATFORK;++i)
      if (pthread_atfork_buf[i].used)
	if (pthread_atfork_buf[i].child)
	  pthread_atfork_buf[i].child();
  }

  __pthread_unlock(&__atfork_struct_lock);
  __NO_ASYNC_CANCEL_END;

  return pid;
}
