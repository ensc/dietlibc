#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>

#define TEST_PATTERN	\
	"0123456789abcdefghijklmnopqrstuv"	\
	"ZYXWVUTSRQPONMLKJIHGFEDCBA987654"	\
	"456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"	\
	"vutsrqponmlkjihgfedcba9876543210"	\
	"0123456789ABCDEFGHIJKLMNOPQRSTUV"	\
	"zyxwvutsrqponmlkjihgfedcba987654"	\
	"456789abcdefghijklmnopqrstuvwxyz"	\
	"VUTSRQPONMLKJIHGFEDCBA987654321"	\

static struct {
	char volatile a[256];
	sigjmp_buf env;
	char volatile b[256];
} sigenv = {
	.a = TEST_PATTERN "<",
	.b = TEST_PATTERN ">",
};

static double const volatile	FP_REF[] = {
  0.4, 0.8, 1.5, 1.6, 2.3, 4.2
};

static int volatile sig_seen;

#define VALIDATE_BUFFERS(_sig_exp) do {		\
    assert(Xmemcmp(sigenv.a, TEST_PATTERN "<", sizeof sigenv.a) == 0);	\
    assert(Xmemcmp(sigenv.b, TEST_PATTERN ">", sizeof sigenv.b) == 0);	\
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
  rc = sigsetjmp(sigenv.env, do_save);
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
      sigenv.a[0] = 'X';
      sigenv.b[0] = 'X';
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

static void do_test_fp(int sig_num, int do_save)
{
  sigset_t	cur_set;
  int		rc;
  double	fp0, fp1, fp2, fp3;

  printf("%s(%d,%d)... ", __func__, sig_num, do_save);
  fflush(stdout);

  VALIDATE_BUFFERS(0);

  assert(sigprocmask(SIG_SETMASK, NULL, &cur_set) == 0);

  /* verify that tested signal is not blocked */
  if (sig_num != 0)
    assert(!sigismember(&cur_set, sig_num));

  fp0 = FP_REF[0];
  fp1 = FP_REF[1];
  fp2 = FP_REF[2];
  fp3 = FP_REF[3];

  sig_seen = 0;
  rc = sigsetjmp(sigenv.env, do_save);

  if (rc == 0) {
    fp0 = FP_REF[4];
    fp3 = FP_REF[5];

    if (sig_num != 0) {
      raise(sig_num);
      assert(0);
    }
  } else if (rc != sig_num)
    /* sigsetjmp() returned with an unexpected value */
    assert(0);

  if (sig_num == 0) {
    /* "exception" was not triggered; we should see the new values */
    assert(fp0 == FP_REF[4]);
    assert(fp1 == FP_REF[1]);
    assert(fp2 == FP_REF[2]);
    assert(fp3 == FP_REF[5]);
  } else {
    /* "exception" was triggered; we should see the old values */
    assert(fp0 == FP_REF[0]);
    assert(fp1 == FP_REF[1]);
    assert(fp2 == FP_REF[2]);
    assert(fp3 == FP_REF[3]);
  }

  VALIDATE_BUFFERS(sig_num);
  sig_seen = 0;

  printf(" ok\n");
}

static void sig_handler(int num)
{
  assert(sig_seen == 0);
  sig_seen = num;
  siglongjmp(sigenv.env, num);
}

int main(void)
{
  struct sigaction sigact = {
    .sa_handler	= sig_handler,
    .sa_flags	= SA_NODEFER, /* raised signal will be in blocked mask else */
  };

  /* verify our assumptions about the memory layout */
  assert(sizeof sigenv.a == 256);
  assert(sizeof sigenv.b == 256);
  assert(offsetof(__typeof__(sigenv), env) == sizeof sigenv.a);
  assert(offsetof(__typeof__(sigenv), b)   == sizeof sigenv.a + sizeof sigenv.env);

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

  do_test_fp(0, 0);
  do_test_fp(0, 1);
  do_test_fp(SIGBUS, 0);
  do_test_fp(SIGBUS, 1);

  return EXIT_SUCCESS;
}
