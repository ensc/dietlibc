#include <sys/types.h>

pid_t waitpid(int pid, int * wait_stat, int flags) {
  return wait4(pid, wait_stat, flags, 0);
}
