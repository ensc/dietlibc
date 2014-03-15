#define _LINUX_SOURCE
#include <threads.h>
#include <unistd.h>
#include <sys/futex.h>
#include <sys/tls.h>
#include <stdlib.h>
#include <errno.h>

extern void __munmap_and_exit(void* map,size_t len) __attribute__((noreturn));

void thrd_exit(int res) {
  thrd_t t=thrd_current();
  int flags;

  size_t len;

  if (!t) exit(res);	// moron user called us when no threads are running
  t->res=res;
  if ((flags=__sync_fetch_and_or(&t->flags,4))&4) return;	// shouldn't be possible, but the tear-down-bit was already set
  if (t->join_futex) {	// somebody is waiting; send him a message
    t->join_wait_futex=0;
    if (futex(&t->join_futex,FUTEX_WAKE,1,0,0,0)==1) {	// wake the guy waiting for us
      // We woke somebody up.
      // Give him time to read our exit code.
      int r;
      do {
	r=futex(&t->join_wait_futex,FUTEX_WAIT,0,0,0,0);
      } while (r==EINTR);
    }
    flags|=1;	// mark as detached
  }

  len=t->memsize;
  if (flags&1) {
    // We are detached or somebody already thrd_join'ed us.
    // We can die in piece and free all our resources
    len += sizeof(tcbhead_t)+sizeof(*t);
  } else {
    // Tell thrd_join that we are dead and cleaned up our stack and it
    // should clean up the rest when it's done.
    __sync_fetch_and_or(&t->flags,2);
  }
  /* Problem: we need to unmap the stack and call exit, but we can't
   * touch the stack in between. Unfortunately, calling exit touches the
   * stack. Returning from the munmap syscall also touches the stack. So
   * this needs to be done in assembly language. */
  __munmap_and_exit(t->memstart,len);
}
