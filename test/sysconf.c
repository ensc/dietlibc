#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/wait.h>

#include "../dietpagesize.h"

static long exec_getconf(char const *var)
{
  char	buf[128];
  pid_t	pid;
  int	fd[2];
  int	status;
  ssize_t	l;

  if (pipe(fd)<0 || (pid = fork())<0)
    abort();

  if (pid==0) {
    close(fd[0]);

    if (fd[1]!=1) {
      dup2(fd[1], 1);
      close(fd[1]);
    }

    execlp("getconf", "getconf", var, NULL);
    _exit(1);
  }

  close(fd[1]);
  l = read(fd[0], buf, sizeof(buf)-1);
  if (l<0) {
    perror("read()");
    goto err;
  } else if (l==sizeof(buf)-1)
    goto err;
  close(fd[0]);

  buf[l] = '\0';

  if (waitpid(pid, &status, 0)<0)
    goto err;

  if (!WIFEXITED(status) || WEXITSTATUS(status)!=0)
    goto err;

  return strtol(buf, NULL, 10);

 err:
  kill(pid, SIGKILL);
  abort();
}

static unsigned int do_check(char const *var, long exp)
{
  long	cur = exec_getconf(var);

  if (cur!=exp) {
    fprintf(stderr, "%s mismatch: got %ld, expected %ld\n",
	    var, cur, exp);
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  unsigned int	err = 0;

  assert(sysconf(_SC_PAGESIZE) == __DIET_PAGE_SIZE);
  assert(__DIET_PAGE_SIZE == (1<<__DIET_PAGE_SHIFT));

  err += do_check("PAGE_SIZE", sysconf(_SC_PAGESIZE));
  err += do_check("CLK_TCK",   sysconf(_SC_CLK_TCK));

  return err;
}
