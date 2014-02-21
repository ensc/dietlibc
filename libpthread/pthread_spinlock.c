#define _REENTRANT

#include <errno.h>

#include <time.h>

#include <pthread.h>
#include "thread_internal.h"

#include <sys/atomic.h>
#include <sys/futex.h>

#include <stdio.h>
#include <stdlib.h>
#include <write12.h>

#define NO_CAS

#ifndef NO_CAS
int __kernel_supports_futex;
#endif

void __pthread_lock(struct _pthread_fastlock*lock) {
  int cnt=0;
  struct timespec tm;
#ifndef NO_CAS
  if (__kernel_supports_futex==-1) goto old_and_busted;
  /* if we have CAS, use a futex */
try_again:
#if 0
  if (__atomic_add(&lock->__spinlock,1)==1)
    /* we now hold the lock, there was no contention */
    return;
  __atomic_add(&lock->__spinlock,-1);
#else
  {
    /* slight optimization: if we don't get the lock immediately, busy-loop for a few iterations. */
    size_t i;
    for (i=0; i<10; ++i) {
      if (__CAS(&lock->__spinlock,0,1)==0)
	/* we now hold the lock, there was no contention */
	return;
    }
  }
#endif
  if (__CAS(&lock->__spinlock,1,2)!=1) goto try_again;
  /* if we got here, we did not get the lock. */
#if 0
  {
    char buf[1024];
    sprintf(buf,"thread %u goes FUTEX_WAIT (lock now %u)\n",gettid(),lock->__spinlock);
    write(1,buf,strlen(buf));
  }
#endif
  if (futex(&lock->__spinlock,FUTEX_WAIT,2,NULL,NULL,0)!=0) {
    switch (*__errno_location()) {
    case ENOSYS: /* kernel too old */
      goto old_and_busted;
    case EWOULDBLOCK: /* when futex() looked, the value was no longer -1 */
    case EINTR: /* a signal interrupted us */
    case EDEADLK: /* not in the man page, treat same as EWOULDBLOCK? */
      break;
#if 0
    default:
      {
	char buf[100];
	sprintf(buf,"futex %d",*__errno_location());
	perror(buf);
      }
      exit(1);
#endif
    }
  }
#if 0
  else { /* we waited, the other guy left, we got woken up. */
    char buf[1024];
    sprintf(buf,"thread %u comes out of FUTEX_WAIT (lock now %u)\n",gettid(),lock->__spinlock);
    write(1,buf,strlen(buf));
  }
#endif
  goto try_again;
old_and_busted:
#endif
  while (__testandset(&lock->__spinlock)) {
    if (cnt<MAX_SPIN_COUNT) {
      sched_yield();
      cnt++;
    } else {
      tm.tv_sec=0;
      tm.tv_nsec=SPIN_SLEEP_DURATION;
      __libc_nanosleep(&tm, 0);
      cnt = 0;
    }
  }
}

int __pthread_trylock(struct _pthread_fastlock*lock) {
#ifndef NO_CAS
  if (__kernel_supports_futex==-1) goto old_and_busted;
  /* if we have CAS, use a futex */
  return __CAS(&lock->__spinlock,0,1)!=0;
old_and_busted:
#endif
  return __testandset(&lock->__spinlock);
}

int __pthread_unlock(struct _pthread_fastlock*lock) {
#ifndef NO_CAS
  if (__kernel_supports_futex==-1) goto old_and_busted;
  /* if we have CAS, use a futex */
  if (__CAS(&lock->__spinlock,1,0)!=1) {
    lock->__spinlock=0;
    futex(&lock->__spinlock,FUTEX_WAKE,1,NULL,NULL,0);
#if 0
    {
      char buf[1024];
      sprintf(buf,"thread %u calls FUTEX_WAKE (lock now %u)\n",gettid(),lock->__spinlock);
      write(1,buf,strlen(buf));
    }
#endif
  }
  return 0;
old_and_busted:
#endif
  lock->__spinlock=PTHREAD_SPIN_UNLOCKED;
  return 0;
}

