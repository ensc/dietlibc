#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid(int pid, int * wait_stat, int flags) {
  return wait4(pid, wait_stat, flags, 0);
}
