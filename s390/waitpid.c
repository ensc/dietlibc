static pid_t waitpid(int pid, int * wait_stat, int flags) {
  return sys_wait4(pid, wait_stat, flags, NULL);
}
