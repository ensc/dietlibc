#include "dietstdio.h"

void funlockfile(FILE* f) {
#ifdef WANT_THREAD_SAFE
  pthread_mutex_unlock(&f->m);
#endif
}
