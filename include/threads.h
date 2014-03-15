#ifndef THREAD_H
#define THREAD_H

#include <sys/types.h>
#include <sys/time.h>

typedef struct __thrd_t {
  struct __thrd_t* next;
  char* memstart;	// points to beginning of guard page
  size_t memsize;	// this includes the guard page, the stack, the thread local storage but not tcb and thrd_t data
  pid_t tid;
  /* the thread function and the argument it was called with */
  int (*func)(void*);
  void* arg;
  /* the return value of the function, so it can be read with pthread_join */
  int res;
  /* 1: the thread was detached
   * 2: the thread already exited, stack and thread local data freed
   * 4: being torn down (calls to thrd_detach and thrd_join fail) */
  int flags;
  int join_futex,join_wait_futex;
}* thrd_t;

enum {
  thrd_success = 0,
  thrd_timedout,
  thrd_busy,
  thrd_nomem,
  thrd_error
};

typedef int (*thrd_start_t)(void* arg);

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
thrd_t thrd_current(void);
int thrd_detach(thrd_t thr);
int thrd_equal(thrd_t thr0, thrd_t thr1);
void thrd_exit(int res);
int thrd_join(thrd_t thr, int* res);
int thrd_sleep(const struct timespec* time_point, struct timespec* remaining);
void thrd_yield(void);

#endif
