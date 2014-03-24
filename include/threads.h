#ifndef THREAD_H
#define THREAD_H

#include <sys/types.h>
#include <sys/time.h>

typedef struct __thrd_t {
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

typedef struct __mtx_t {
  int lock, type;
  thrd_t owner;
} mtx_t;

enum {
  mtx_plain = 0,
  mtx_timed = 1,
  mtx_recursive = 2
};

int mtx_init(mtx_t* mutex, int type);
int mtx_lock(mtx_t* mutex);
int mtx_timedlock(mtx_t *restrict mutex, const struct timespec *restrict time_point);
int mtx_trylock(mtx_t* mutex);
int mtx_unlock(mtx_t* mutex);
void mtx_destroy(mtx_t* mutex);

#ifdef _DIETLIBC_SOURCE
int __mtx_trylock(mtx_t* mutex,int* lockval);
#endif

typedef int once_flag;
#define ONCE_FLAG_INIT 0

void call_once(once_flag* flag, void (*func)(void));

typedef struct __cnd_t {
  int sem;
} cnd_t;

int cnd_init(cnd_t* cond);
int cnd_signal(cnd_t *cond);
int cnd_broadcast(cnd_t *cond);
int cnd_wait(cnd_t* cond, mtx_t* mutex);
int cnd_timedwait(cnd_t* restrict cond, mtx_t* restrict mutex, const struct timespec* restrict time_point);
void cnd_destroy(cnd_t* cond);

#define thread_local __thread

typedef void (*tss_dtor_t)(void*);
typedef struct __tss_t {
  void* data;
  tss_dtor_t dtor;
  struct __tss_t* next;
}* tss_t;

#define TSS_DTOR_ITERATIONS 1
int tss_create(tss_t* tss_id, tss_dtor_t destructor);
void *tss_get(tss_t tss_id);
int tss_set(tss_t tss_id, void *val);
void tss_delete(tss_t tss_id);

#endif
