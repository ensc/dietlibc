#include <unistd.h>
#include <errno.h>
#include "dietfeatures.h"

int remove(const char* filename) {
  if (unlink(filename)) {
#ifdef WANT_THREAD_SAFE
    if (*(__errno_location())==EISDIR)
#else
    if (errno==EISDIR)
#endif
      return rmdir(filename);
    return -1;
  }
  return 0;
}
