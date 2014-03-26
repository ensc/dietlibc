#include <threads.h>
#include <sys/tls.h>
#include <sys/mman.h>

int thrd_detach(thrd_t thr) {
  int flags=thr->flags;
  do {
    if (flags&4) return thrd_error;
    if (flags&2) {
      munmap(thr->memstart+thr->memsize,sizeof(tcbhead_t)+sizeof(*thr));
      return thrd_success;
    }
    if (flags&1) return thrd_success;
    flags=__sync_val_compare_and_swap(&thr->flags,flags,flags|1);
  } while (1);
}

