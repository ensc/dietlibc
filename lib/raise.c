#include <linux/types.h>
#include <unistd.h>
#include <signal.h>
#include "syscalls.h"

int raise(int sig) {
  return kill(getpid(),sig);
}
