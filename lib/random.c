#include <stdlib.h>

static unsigned int seed=1;

int random(void) {
  return ((seed = seed * 1103515245 + 12345) % ((unsigned int)RAND_MAX + 1));
}

void srandom(unsigned int i) { seed=i; }
