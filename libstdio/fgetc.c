#include "dietstdio.h"
#include <unistd.h>

#ifdef WANT_THREAD_SAFE
int fgetc(FILE *stream) {
  int tmp;
  pthread_mutex_lock(&stream->m);
  tmp=fgetc_unlocked(stream);
  pthread_mutex_unlock(&stream->m);
  return tmp;
}
#endif
