#include <stdlib.h>
#include <sys/auxv.h>
#include "rand_i.h"

static unsigned int seed;

int rand_i(void) {
  if (seed==0) {
    const int* tmp=(const int*)getauxval(AT_RANDOM);
    if (tmp)
      seed=tmp[3];
    else
      seed=(uintptr_t)&seed;		// really old crappy kernel
  }
  return rand_r(&seed);
}

