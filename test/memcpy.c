#include <string.h>
#include <assert.h>

main() {
  char test[100]="blubber";
  assert(memcpy(test,"blubber",8)==test);
  assert(!memcmp(test,"blubber",8));
  assert(memcpy(0,0,0)==0);
  return 0;
}
