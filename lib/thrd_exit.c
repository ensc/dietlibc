#define _REENTRANT
#define _LINUX_SOURCE
#include <threads.h>
#include <unistd.h>
#include <sys/futex.h>
#include <sys/tls.h>
#include <stdlib.h>
#include <errno.h>

extern void __munmap_and_exit(void* map,size_t len) __attribute__((noreturn));

thread_local tss_t tss_dtor_list;

void thrd_exit(int res) {
  thrd_t t=thrd_current();
  int flags;

  size_t len;

  while (tss_dtor_list) {
    tss_t cur=tss_dtor_list;
    tss_dtor_list=tss_dtor_list->next;
    if (cur->dtor) cur->dtor(cur);
  }
  if (!t) exit(res);	// moron user called us when no threads are running
  t->res=res;
  if ((flags=__sync_fetch_and_or(&t->flags,4))&4) return;	// shouldn't be possible, but the tear-down-bit was already set
  if (t->join_futex) {	// somebody is waiting; send him a message
    t->join_wait_futex=0;
    /* we need to wake the waiting threads up one by one, because we
     * need to wait for all of them to have received our return value
     * before we can self destruct. */
    while (futex(&t->join_futex,FUTEX_WAKE,1,0,0,0)==1) {	// wake one waiting thread
      // We woke somebody up.
      // Give them time to read our exit code.
      int r;
      do {
	r=futex(&t->join_wait_futex,FUTEX_WAIT,0,0,0,0);
      } while (r==EINTR);
      t->join_wait_futex=0;
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
    len&=~4095;	// round down to nearest page
    t->arg=t->memstart+len;
    t->memsize=t->memsize+sizeof(tcbhead_t)+sizeof(*t)-len;
  }
  /* Problem: we need to unmap the stack and call exit, but we can't
   * touch the stack in between. Unfortunately, calling exit touches the
   * stack. Returning from the munmap syscall also touches the stack. So
   * this needs to be done in assembly language. */
  __munmap_and_exit(t->memstart,len);
}
