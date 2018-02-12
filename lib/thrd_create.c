#define _LINUX_SOURCE
#include <unistd.h>
#include <threads.h>
#include <sys/mman.h>
#include <fcntl.h>
#define _GNU_SOURCE
#include <sched.h>
#include "dietfeatures.h"
#include <sys/tls.h>
#include <stdio.h>
#include <sys/random.h>

extern thrd_t _thrd_root;
extern size_t _thrd_stacksize;

#ifdef WANT_TLS
extern size_t __tdatasize, __tmemsize;
extern void* __tdataptr;
extern tcbhead_t* __tcb_mainthread;
#else
static const size_t __tmemsize=0,__tdatasize=0;
#endif

static int launch(void* p) {
  thrd_t self=p;
  thrd_exit(self->func(self->arg));
  return 0;
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
  size_t stacksize=_thrd_stacksize;	// capture value to prevent asynchronous changes to break us
  char* stack=mmap(0,stacksize+4096+__tmemsize+sizeof(tcbhead_t)+sizeof(**thr),PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK|MAP_GROWSDOWN,-1,0);
  thrd_t t;
  tcbhead_t* tcb;

  if (stack==MAP_FAILED) return thrd_nomem;
  if (mprotect(stack+4096,stacksize+__tmemsize+sizeof(tcbhead_t)+sizeof(**thr),PROT_READ|PROT_WRITE)) {
    munmap(stack,stacksize+4096+__tmemsize+sizeof(tcbhead_t)+sizeof(**thr));
    return thrd_error;
  }
  tcb=(tcbhead_t*)(stack+stacksize+4096+__tmemsize);
  tcb->tcb=tcb;
  tcb->dtv=0;
  t=(thrd_t)(tcb+1);
  tcb->self=t;
  tcb->multiple_threads=1;
#ifdef WANT_TLS
  tcb->sysinfo=__tcb_mainthread->sysinfo;
  if (getrandom(&tcb->stack_guard,sizeof(tcb->stack_guard),GRND_NONBLOCK) != sizeof(tcb->stack_guard))
    tcb->stack_guard=__tcb_mainthread->stack_guard ^ gettid() ^ (uintptr_t)t;
  __tcb_mainthread->multiple_threads=1;
#endif
  t->memstart=stack;
  t->memsize=stacksize+4096+__tmemsize;
  t->res=0;
  t->func=func;
  t->arg=arg;
  t->flags=0;
  t->join_futex=0;

#ifdef WANT_TLS
  memcpy(stack+stacksize+4096,__tdataptr,__tdatasize);
  memset(stack+stacksize+4096+__tdatasize,0,__tmemsize-__tdatasize);
#endif

  t->tid=clone(launch,stack+4096+stacksize,CLONE_FILES|CLONE_FS|CLONE_IO|CLONE_PARENT|CLONE_SIGHAND|CLONE_SYSVSEM|CLONE_THREAD|CLONE_VM|CLONE_SETTLS|CLONE_PARENT_SETTID,t,&t->tid,stack+stacksize+4096+__tmemsize);
  if (t->tid==-1) {
    munmap(stack,stacksize+4096+__tmemsize+sizeof(tcbhead_t)+sizeof(**thr));
    return thrd_error;
  }
  *thr=t;
  return thrd_success;
}
