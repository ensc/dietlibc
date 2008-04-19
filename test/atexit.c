#include <stdlib.h>
#include <unistd.h>

void blah(void) {
  write(1,"atexit\n",7);
}

int main() {
  atexit(blah);
  return 0;
}
