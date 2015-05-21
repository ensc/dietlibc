#define _LINUX_SOURCE
#include <unistd.h>
#include <stdlib.h>

static uint32_t buf[256];
static unsigned int n;

/* These come from OpenBSD: */
uint32_t arc4random(void) {
  if (n+1>sizeof(buf)) arc4random_stir();
  return buf[n++];
}

void arc4random_buf(void* Buf, size_t N) {
  if ((size_t)getrandom(Buf,N,GRND_NONBLOCK) != N)
    abort();
}

uint32_t arc4random_uniform(uint32_t upper_bound) {
  uint32_t r, min;

  if (upper_bound < 2)
    return 0;

  /* 2**32 % x == (2**32 - x) % x */
  min = -upper_bound % upper_bound;

  /*
    * This could theoretically loop forever but each retry has
    * p > 0.5 (worst case, usually far better) of selecting a
    * number inside the range we need, so it should rarely need
    * to re-roll.
    */
  for (;;) {
    r = arc4random();
    if (r >= min)
      break;
  }

  return r % upper_bound;
}

void arc4random_stir(void) {
  n=0;
  if ((size_t)getrandom(buf,sizeof(buf),GRND_NONBLOCK) != sizeof(buf))
    abort();
}

void arc4random_addrandom(unsigned char* dat,size_t datlen) {
}

