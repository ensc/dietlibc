#include <unistd.h>

pid_t vfork() __attribute__((alias("__libc_fork")));

