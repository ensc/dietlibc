#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "exec_lib.h"
#include "dietfeatures.h"

int execv(const char *file, char *const argv[]) {
  if (execve(file,argv,environ)==-1) {
#ifdef WANT_THREAD_SAFE
    if (*(__errno_location())==ENOEXEC)
#else
    if (errno==ENOEXEC)
#endif
      __exec_shell(file,argv);
  }
  return -1;
}
