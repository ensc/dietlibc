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

  if (1) {
    char *shell_argv[i + 1];
    shell_argv[0] = _PATH_BSHELL;
    shell_argv[1] = (char *) file;
    for (; i > 1; i--)
      shell_argv[i] = argv[i - 1];
    return execve(_PATH_BSHELL, shell_argv, environ);
  }
}

int execvp(const char *file, char *const argv[]) {
  char *path=getenv("PATH");
  char *cur,*next;
  char buf[PATH_MAX];
  if (strchr((char*)file,'/')) {
    if (execve(file,argv,environ)==-1) {
      if (errno==ENOEXEC) exec_shell(file,argv);
      return -1;
    }
  }
  if (!path) path=_PATH_DEFPATH;
  for (cur=path; cur; cur=next) {
    next=strchr(cur,':');
    if (!next)
      next=cur+strlen(cur);
    if (next==cur) {
      buf[0]='.';
      cur--;
    } else
      memmove(buf,cur,next-cur);
    buf[next-cur]='/';
    memmove(&buf[next-cur+1],file,strlen(file)+1);
    if (execve(buf,argv,environ)==-1) {
      if (errno==ENOEXEC) return exec_shell(buf,argv);
      if ((errno!=EACCES) && (errno!=ENOENT)) return -1;
    }
    if (*next==0) break;
    next++;
  }
  return -1;
}
