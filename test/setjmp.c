#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

static int Xmemcmp(void const volatile *a, void const volatile *b, size_t l)
{
  return memcmp((void const *)a, (void const *)b, l);
}

int main(void)
{
  char volatile a[8] = "testbufA";
  jmp_buf env;
  char volatile b[8] = "testbufB";

  assert(Xmemcmp(a, "testbufA", 8) == 0);
  assert(Xmemcmp(b, "testbufB", 8) == 0);


  /* Test 1: not calling longjmp */
  if (setjmp(env) == 0) {
    char volatile somebuf[128];

    memset((void *)somebuf, 0xde, sizeof somebuf);

    assert(Xmemcmp(a, "testbufA", 8) == 0);
    assert(Xmemcmp(b, "testbufB", 8) == 0);
  } else
    assert(0);

  assert(Xmemcmp(a, "testbufA", 8) == 0);
  assert(Xmemcmp(b, "testbufB", 8) == 0);


  /* Test 2: calling longjmp */
  switch (setjmp(env)) {
  case 0: {
    char volatile somebuf[128];

    memset((void *)somebuf, 0xde, sizeof somebuf);

    assert(Xmemcmp(a, "testbufA", 8) == 0);
    assert(Xmemcmp(b, "testbufB", 8) == 0);

    longjmp(env, 23);

    a[0] = 'X';
    b[0] = 'X';
  }

  case 23:
    break;

  default:
    assert(0);
  }

  assert(Xmemcmp(a, "testbufA", 8) == 0);
  assert(Xmemcmp(b, "testbufB", 8) == 0);


  /* Test 3: calling longjmp again with dirty env */
  switch (setjmp(env)) {
  case 0: {
    char volatile somebuf[128];

    memset((void *)somebuf, 0xde, sizeof somebuf);

    assert(Xmemcmp(a, "testbufA", 8) == 0);
    assert(Xmemcmp(b, "testbufB", 8) == 0);

    longjmp(env, 23);

    a[0] = 'X';
    b[0] = 'X';
  }

  case 23:
    break;

  default:
    assert(0);
  }

  assert(Xmemcmp(a, "testbufA", 8) == 0);
  assert(Xmemcmp(b, "testbufB", 8) == 0);


 /* Test 4: not calling longjmp, but dirty env */
  if (setjmp(env) == 0) {
    char volatile somebuf[128];

    memset((void *)somebuf, 0xde, sizeof somebuf);

    assert(Xmemcmp(a, "testbufA", 8) == 0);
    assert(Xmemcmp(b, "testbufB", 8) == 0);
  } else
    assert(0);

  assert(Xmemcmp(a, "testbufA", 8) == 0);
  assert(Xmemcmp(b, "testbufB", 8) == 0);


  return EXIT_SUCCESS;
}
