#include <stdlib.h>

static unsigned int seed=1;

int rand(void) {
  return ((seed = seed * 1103515245 + 12345) % ((unsigned int)RAND_MAX + 1));
}

void srand(unsigned int i) { seed=i; }

int random(void) __attribute__((alias("rand")));
void srandom(unsigned int i) __attribute__((alias("srand")));
