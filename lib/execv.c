#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <paths.h>
#include <errno.h>

extern char **environ;

static int exec_shell(const char *file, char *const argv[]) {
  int i;

  for (i = 0; argv[i]; i++);

  {
    char *shell_argv[i + 1];
    shell_argv[0] = _PATH_BSHELL;
    shell_argv[1] = (char *) file;
    for (; i > 1; i--)
      shell_argv[i] = argv[i - 1];
    return execve(_PATH_BSHELL, shell_argv, environ);
  }
}

int execv(const char *file, char *const argv[]) {
  if (execve(file,argv,environ)==-1) {
    if (errno==ENOEXEC) exec_shell(file,argv);
  }
  return -1;
}
