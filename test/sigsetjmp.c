#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>

static char volatile a[8] = "testbufA";
static sigjmp_buf env;
static char volatile b[8] = "testbufB";
static int volatile sig_seen;

#define VALIDATE_BUFFERS(_sig_exp) do {		\
    assert(Xmemcmp(a, "testbufA", 8) == 0);	\
    assert(Xmemcmp(b, "testbufB", 8) == 0);	\
    assert(sig_seen == (_sig_exp));		\
  } while (0)

static int Xmemcmp(void const volatile *a, void const volatile *b, size_t l)
{
  return memcmp((void const *)a, (void const *)b, l);
}

static void do_test(int sig_num, int do_save, int block_sig)
{
  int		rc;
  sigset_t	block_set;
  sigset_t	cur_set;

  printf("%s(%d,%d,%d)... ", __func__, sig_num, do_save, block_sig);
  fflush(stdout);

  VALIDATE_BUFFERS(0);

  sigemptyset(&block_set);
  assert(sigprocmask(SIG_SETMASK, NULL, &cur_set) == 0);

  /* verify that tested signal is not blocked */
  if (sig_num != 0)
    assert(!sigismember(&cur_set, sig_num));

  /* verify that blocked signal is not already blocked and fill signal set */
  if (block_sig != 0) {
    assert(!sigismember(&cur_set, block_sig));
    sigaddset(&block_set, block_sig);
  }

  sig_seen = 0;
  rc = sigsetjmp(env, do_save);
  if (rc == 0) {
    char volatile somebuf[128];

    memset((void *)somebuf, 0x42, sizeof somebuf);
    VALIDATE_BUFFERS(0);

    /* modify signal mask */
    if (block_sig != 0)
      assert(sigprocmask(SIG_BLOCK, &block_set, NULL) == 0);

    /* raise a signal which triggers a siglongjmp */
    if (sig_num != 0) {
      raise(sig_num);
      a[0] = 'X';
      b[0] = 'X';
      assert(0);
    }
  } else if (rc != sig_num)
    /* sigsetjmp() returned with an unexpected value */
    assert(0);

  VALIDATE_BUFFERS(sig_num);
  sig_seen = 0;

  /* check whether current signal mask contains the blocked signal; it should
     be there iff sigsetjmp() was triggered and sigmask was saved. */
  if (block_sig != 0) {
    sigset_t	cur_set;
    assert(sigprocmask(SIG_SETMASK, NULL, &cur_set) == 0);

    if (do_save && rc != 0)
      assert(!sigismember(&cur_set, block_sig));
    else {
      assert( sigismember(&cur_set, block_sig));
      sigprocmask(SIG_UNBLOCK, &block_set, NULL);
    }
  }

  printf(" ok\n");
};

static void sig_handler(int num)
{
  assert(sig_seen == 0);
  sig_seen = num;
  siglongjmp(env, num);
}

int main(void)
{
  struct sigaction sigact = {
    .sa_handler	= sig_handler,
    .sa_flags	= SA_NODEFER, /* raised signal will be in blocked mask else */
  };

  sigaction(SIGBUS,  &sigact, NULL);
  sigaction(SIGUSR1, &sigact, NULL);

  do_test(0, 0, 0);
  do_test(0, 0, SIGUSR1);
  do_test(0, 1, 0);
  do_test(0, 1, SIGUSR1);
  do_test(SIGBUS, 0, 0);
  do_test(SIGBUS, 0, SIGUSR1);
  do_test(SIGBUS, 1, 0);
  do_test(SIGBUS, 1, SIGUSR1);

  return EXIT_SUCCESS;
}
