#include "dietstdio.h"

void flockfile(FILE* f) {
#ifdef WANT_THREAD_SAFE
  pthread_mutex_lock(&f->m);
#endif
}
