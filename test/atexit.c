#include <stdlib.h>
#include <unistd.h>

void blah(void) {
  write(2,"atexit\n",7);
}

main() {
  atexit(blah);
}
